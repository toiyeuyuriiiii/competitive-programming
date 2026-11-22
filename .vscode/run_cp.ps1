param(
    [Parameter(Mandatory = $true)]
    [string]$src,

    [Parameter(Mandatory = $true)]
    [string]$exe
)

chcp 65001 | Out-Null
[Console]::InputEncoding  = [System.Text.UTF8Encoding]::new($false)
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new($false)
$OutputEncoding           = [System.Text.UTF8Encoding]::new($false)

if ([System.IO.Path]::GetExtension($src) -ine ".cpp") {
    Write-Host "[ERROR] Unsupported file type. Expected a C++ source file (.cpp)." -ForegroundColor Red
    exit 1
}

Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;

public class Win32Mem {
    [StructLayout(LayoutKind.Sequential)]
    public struct PROCESS_MEMORY_COUNTERS {
        public uint cb;
        public uint PageFaultCount;
        public UIntPtr PeakWorkingSetSize;
        public UIntPtr WorkingSetSize;
        public UIntPtr QuotaPeakPagedPoolUsage;
        public UIntPtr QuotaPagedPoolUsage;
        public UIntPtr QuotaPeakNonPagedPoolUsage;
        public UIntPtr QuotaNonPagedPoolUsage;
        public UIntPtr PagefileUsage;
        public UIntPtr PeakPagefileUsage;
    }

    [DllImport("psapi.dll", SetLastError = true)]
    public static extern bool GetProcessMemoryInfo(IntPtr hProcess, out PROCESS_MEMORY_COUNTERS counters, uint size);

    public static ulong GetPeakWorkingSet(IntPtr handle) {
        PROCESS_MEMORY_COUNTERS pmc = new PROCESS_MEMORY_COUNTERS();
        pmc.cb = (uint)Marshal.SizeOf(typeof(PROCESS_MEMORY_COUNTERS));
        if (GetProcessMemoryInfo(handle, out pmc, pmc.cb)) {
            return (ulong)pmc.PeakWorkingSetSize;
        }
        return 0;
    }
}
"@

$workDir = Split-Path -Parent $src
$fileName = Split-Path -Leaf $src
Set-Location -Path $workDir

if (Test-Path $exe) {
    Remove-Item -Path $exe -Force
}

$flags = @(
    "-std=c++14", "-O2", "-Wall", "-Wextra",
    "-DLOCAL", "-Wshadow", "-Wfatal-errors",
    "-D_GLIBCXX_DEBUG", "-Wl,--stack,1073741824"
)

$errLog = Join-Path $env:TEMP "gpp_err_$([guid]::NewGuid().ToString('N')).txt"
$argList = $flags + @($src, "-o", $exe)

$buildSw = [System.Diagnostics.Stopwatch]::StartNew()
$buildProc = Start-Process -FilePath "g++" -ArgumentList $argList -RedirectStandardError $errLog -NoNewWindow -PassThru
$buildHandle = $buildProc.Handle

$frames = @(0x280B, 0x2819, 0x2839, 0x2838, 0x283C, 0x2834, 0x2826, 0x2827, 0x2807, 0x280F) | ForEach-Object { [char]$_ }
$idx = 0
[Console]::CursorVisible = $false

try {
    while (-not $buildProc.HasExited) {
        $elapsedMs = $buildSw.ElapsedMilliseconds
        $frame = $frames[$idx % $frames.Length]
        Write-Host ("`r{0} [BUILD] Compiling {1}... ({2} ms)   " -f $frame, $fileName, $elapsedMs) -NoNewline -ForegroundColor Cyan
        Start-Sleep -Milliseconds 50
        $idx++
    }
}
finally {
    [Console]::CursorVisible = $true
}

$buildProc.WaitForExit()
$buildSw.Stop()
$buildCode = $buildProc.ExitCode
$buildProc.Dispose()
$buildMs = $buildSw.ElapsedMilliseconds

Write-Host ("`r" + (" " * 65) + "`r") -NoNewline

if ($buildCode -ne 0) {
    Write-Host ("[BUILD FAILED] Compilation failed in {0} ms (Exit Code: {1})" -f $buildMs, $buildCode) -ForegroundColor Red
    if (Test-Path $errLog) {
        Get-Content $errLog | ForEach-Object { Write-Host $_ }
        Remove-Item $errLog -Force
    }
    exit $buildCode
}

if (Test-Path $errLog) {
    $warnings = Get-Content $errLog
    if ($warnings) {
        $warnings | ForEach-Object { Write-Host $_ -ForegroundColor Yellow }
    }
    Remove-Item $errLog -Force
}

Write-Host ("[BUILD OK] Compiled {0} in {1} ms" -f $fileName, $buildMs) -ForegroundColor Green

Write-Host "[RUN] Executing binary..." -ForegroundColor Cyan
Write-Host "`n================ PROGRAM I/O ================" -ForegroundColor DarkGray

$sw = [System.Diagnostics.Stopwatch]::StartNew()
$proc = Start-Process -FilePath $exe -WorkingDirectory $workDir -NoNewWindow -PassThru
$handle = $proc.Handle
$proc.WaitForExit()
$sw.Stop()

$peakBytes = [Win32Mem]::GetPeakWorkingSet($handle)
$peakMemMB = [math]::Round($peakBytes / 1MB, 2)
$timeMs = $sw.ElapsedMilliseconds
$exitCode = $proc.ExitCode
$proc.Dispose()

Write-Host "`n=============================================" -ForegroundColor DarkGray
$status = if ($exitCode -eq 0) { "SUCCESS" } else { "RUNTIME ERROR" }
$color = if ($exitCode -eq 0) { "Green" } else { "Red" }

Write-Host ("[{0}] Time: {1} ms | Peak Memory: {2:N2} MB | Exit Code: {3}" -f $status, $timeMs, $peakMemMB, $exitCode) -ForegroundColor $color