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

    public static ulong GetCurrentWorkingSet(IntPtr handle) {
        PROCESS_MEMORY_COUNTERS pmc = new PROCESS_MEMORY_COUNTERS();
        pmc.cb = (uint)Marshal.SizeOf(typeof(PROCESS_MEMORY_COUNTERS));
        if (GetProcessMemoryInfo(handle, out pmc, pmc.cb)) {
            return (ulong)pmc.WorkingSetSize;
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

$esc = [char]27
Write-Host "[RUN] Executing binary..." -ForegroundColor Cyan
$statusRow = [Console]::CursorTop + 1
Write-Host "  [LIVE] Time: 0 ms | RAM: 0.00 MB (Peak: 0.00 MB / 1024 MB)" -ForegroundColor Yellow
Write-Host "================ PROGRAM I/O ================" -ForegroundColor DarkGray

$memLimitBytes = 1GB
$mleKilled = $false
$idx = 0

$sw = [System.Diagnostics.Stopwatch]::StartNew()
$proc = Start-Process -FilePath $exe -WorkingDirectory $workDir -NoNewWindow -PassThru
$handle = $proc.Handle

while (-not $proc.HasExited) {
    $curBytes = [Win32Mem]::GetCurrentWorkingSet($handle)
    $peakBytes = [Win32Mem]::GetPeakWorkingSet($handle)
    
    if ($curBytes -gt $memLimitBytes -or $peakBytes -gt $memLimitBytes) {
        $mleKilled = $true
        try { $proc.Kill() } catch {}
        break
    }

    $curMB = [math]::Round($curBytes / 1MB, 2)
    $peakMB = [math]::Round($peakBytes / 1MB, 2)
    $frame = $frames[$idx % $frames.Length]
    $idx++

    $liveText = "{0} [LIVE] Time: {1} ms | RAM: {2:N2} MB (Peak: {3:N2} MB / 1024 MB)" -f $frame, $sw.ElapsedMilliseconds, $curMB, $peakMB
    [Console]::Write("$esc" + "7" + "$esc[$statusRow;1H" + "$esc[2K" + "$esc[33m$liveText$esc[0m" + "$esc" + "8")

    Start-Sleep -Milliseconds 40
}

$proc.WaitForExit()
$sw.Stop()

$peakBytes = [Win32Mem]::GetPeakWorkingSet($handle)
$peakMemMB = [math]::Round($peakBytes / 1MB, 2)
$timeMs = $sw.ElapsedMilliseconds
$exitCode = if ($mleKilled) { -1 } else { $proc.ExitCode }
$proc.Dispose()

$finalLive = if ($mleKilled) {
    "$esc[31m✖ [STOPPED] Memory Limit Exceeded (> 1024 MB)$esc[0m"
} else {
    "$esc[32m✔ [DONE] Time: $timeMs ms | Peak RAM: $($peakMemMB.ToString('N2')) MB$esc[0m"
}
[Console]::Write("$esc" + "7" + "$esc[$statusRow;1H" + "$esc[2K" + $finalLive + "$esc" + "8")

Write-Host "`n=============================================" -ForegroundColor DarkGray
if ($mleKilled) {
    Write-Host ("[MEMORY LIMIT EXCEEDED] Killed (> 1024 MB) | Time: {0} ms | Peak Memory: {1:N2} MB" -f $timeMs, $peakMemMB) -ForegroundColor Red
} else {
    $status = if ($exitCode -eq 0) { "SUCCESS" } else { "RUNTIME ERROR" }
    $color = if ($exitCode -eq 0) { "Green" } else { "Red" }
    Write-Host ("[{0}] Time: {1} ms | Peak Memory: {2:N2} MB | Exit Code: {3}" -f $status, $timeMs, $peakMemMB, $exitCode) -ForegroundColor $color
}