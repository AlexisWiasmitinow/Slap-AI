# Build and Upload Script for Windows PowerShell
# WEMOS S3 MINI PRO - IMU Display Test

Write-Host "`n=== WEMOS S3 MINI PRO Build Script ===`n" -ForegroundColor Cyan

# Check if PlatformIO is installed
Write-Host "Checking for PlatformIO..." -ForegroundColor Yellow
$pioVersion = pio --version 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: PlatformIO not found!" -ForegroundColor Red
    Write-Host "Install it with: pip install platformio" -ForegroundColor Yellow
    exit 1
}
Write-Host "PlatformIO found: $pioVersion" -ForegroundColor Green

# Show menu
Write-Host "`nWhat would you like to do?" -ForegroundColor Cyan
Write-Host "1. Build only" -ForegroundColor White
Write-Host "2. Build and upload" -ForegroundColor White
Write-Host "3. Build, upload, and monitor" -ForegroundColor White
Write-Host "4. Clean build" -ForegroundColor White
Write-Host "5. Monitor serial only" -ForegroundColor White
Write-Host "6. Erase flash (removes MicroPython)" -ForegroundColor White
Write-Host "7. Exit" -ForegroundColor White

$choice = Read-Host "`nEnter choice (1-7)"

switch ($choice) {
    "1" {
        Write-Host "`n--- Building project ---" -ForegroundColor Cyan
        pio run
    }
    "2" {
        Write-Host "`n--- Building and uploading ---" -ForegroundColor Cyan
        Write-Host "`n⚠️  IMPORTANT: When you see 'Connecting...', HOLD the BOOT button (GPIO0)!" -ForegroundColor Yellow
        Start-Sleep -Seconds 2
        pio run --target upload
    }
    "3" {
        Write-Host "`n--- Building, uploading, and monitoring ---" -ForegroundColor Cyan
        Write-Host "`n⚠️  IMPORTANT: When you see 'Connecting...', HOLD the BOOT button (GPIO0)!" -ForegroundColor Yellow
        Start-Sleep -Seconds 2
        pio run --target upload && pio device monitor
    }
    "4" {
        Write-Host "`n--- Cleaning and building ---" -ForegroundColor Cyan
        pio run --target clean
        pio run
    }
    "5" {
        Write-Host "`n--- Starting serial monitor (115200 baud) ---" -ForegroundColor Cyan
        Write-Host "Press Ctrl+C to exit" -ForegroundColor Yellow
        pio device monitor
    }
    "6" {
        Write-Host "`n--- Erasing flash memory ---" -ForegroundColor Cyan
        Write-Host "⚠️  This will remove MicroPython completely!" -ForegroundColor Yellow
        Write-Host "⚠️  HOLD the BOOT button (GPIO0) NOW!" -ForegroundColor Yellow
        Start-Sleep -Seconds 3
        pio run --target erase
        Write-Host "`nFlash erased! Now you can upload normally." -ForegroundColor Green
    }
    "7" {
        Write-Host "`nExiting..." -ForegroundColor Yellow
        exit 0
    }
    default {
        Write-Host "`nInvalid choice!" -ForegroundColor Red
        exit 1
    }
}

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n=== SUCCESS ===" -ForegroundColor Green
} else {
    Write-Host "`n=== FAILED ===" -ForegroundColor Red
    Write-Host "`nTroubleshooting tips:" -ForegroundColor Yellow
    Write-Host "1. Hold BOOT button (GPIO0) while uploading" -ForegroundColor White
    Write-Host "2. Try different USB cable" -ForegroundColor White
    Write-Host "3. Check COM port in Device Manager" -ForegroundColor White
    Write-Host "4. Install CP210x or CH340 USB drivers if needed" -ForegroundColor White
}

Write-Host ""


