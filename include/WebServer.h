#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <Update.h>

// Define HTTP methods if not already defined
#ifndef HTTP_GET
typedef enum {
  HTTP_GET = 0b00000001,
  HTTP_POST = 0b00000010,
  HTTP_DELETE = 0b00000100,
  HTTP_PUT = 0b00001000,
  HTTP_PATCH = 0b00010000,
  HTTP_HEAD = 0b00100000,
  HTTP_OPTIONS = 0b01000000,
  HTTP_ANY = 0b01111111,
} WebRequestMethod;
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "WiFiManager.h"

class SlapWebServer {
 private:
  AsyncWebServer *server;
  ConfigManager *configMgr;
  SlapWiFiManager *wifiMgr;

  // HTML page with embedded CSS and JavaScript
  const char *getIndexHTML() {
    return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Slap-AI Settings</title>
    <link rel="icon" type="image/x-icon" href="data:image/x-icon;base64,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">
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .container {
            background: white;
            border-radius: 16px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            padding: 30px;
            max-width: 500px;
            width: 100%;
        }
        h1 {
            color: #667eea;
            margin-bottom: 10px;
            font-size: 28px;
            text-align: center;
        }
        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 14px;
        }
        .status {
            background: #f0f4ff;
            padding: 15px;
            border-radius: 8px;
            margin-bottom: 25px;
        }
        .status-row {
            display: flex;
            justify-content: space-between;
            margin-bottom: 8px;
            font-size: 14px;
        }
        .status-label { color: #666; }
        .status-value { 
            font-weight: 600;
            color: #333;
        }
        .status-value.connected { color: #10b981; }
        .status-value.ap { color: #f59e0b; }
        .section {
            margin-bottom: 25px;
        }
        .section-title {
            font-size: 16px;
            font-weight: 600;
            color: #333;
            margin-bottom: 12px;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        .input-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 6px;
            color: #555;
            font-size: 14px;
            font-weight: 500;
        }
        input[type="text"],
        input[type="password"],
        input[type="number"] {
            width: 100%;
            padding: 10px 12px;
            border: 2px solid #e5e7eb;
            border-radius: 8px;
            font-size: 14px;
            transition: border-color 0.2s;
        }
        input:focus {
            outline: none;
            border-color: #667eea;
        }
        .slider-group {
            margin-bottom: 20px;
        }
        .slider-value {
            text-align: center;
            font-size: 24px;
            font-weight: 700;
            color: #667eea;
            margin-bottom: 10px;
        }
        input[type="range"] {
            width: 100%;
            height: 6px;
            border-radius: 5px;
            background: #e5e7eb;
            outline: none;
            -webkit-appearance: none;
        }
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
        }
        input[type="range"]::-moz-range-thumb {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            border: none;
        }
        .button {
            width: 100%;
            padding: 12px;
            border: none;
            border-radius: 8px;
            font-size: 15px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s;
            margin-bottom: 10px;
        }
        .button-primary {
            background: #667eea;
            color: white;
        }
        .button-primary:hover {
            background: #5568d3;
            transform: translateY(-1px);
            box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
        }
        .button-danger {
            background: #ef4444;
            color: white;
        }
        .button-danger:hover {
            background: #dc2626;
        }
        .message {
            padding: 12px;
            border-radius: 8px;
            margin-bottom: 15px;
            font-size: 14px;
            display: none;
        }
        .message.success {
            background: #d1fae5;
            color: #065f46;
            display: block;
        }
        .message.error {
            background: #fee2e2;
            color: #991b1b;
            display: block;
        }
        .dropzone {
            border: 3px dashed #cbd5e1;
            border-radius: 12px;
            padding: 40px 20px;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s ease;
            background: #f8fafc;
            margin-bottom: 15px;
        }
        .dropzone:hover {
            border-color: #667eea;
            background: #f0f4ff;
        }
        .dropzone.dragover {
            border-color: #667eea;
            background: #e0e7ff;
            box-shadow: 0 0 0 4px rgba(102, 126, 234, 0.1);
            transform: scale(1.02);
        }
        .dropzone-icon {
            font-size: 48px;
            margin-bottom: 10px;
            opacity: 0.7;
        }
        .dropzone.dragover .dropzone-icon {
            animation: pulse 1s infinite;
        }
        @keyframes pulse {
            0%, 100% { transform: scale(1); opacity: 0.7; }
            50% { transform: scale(1.1); opacity: 1; }
        }
        .dropzone-text {
            color: #64748b;
            font-size: 16px;
            font-weight: 500;
            margin-bottom: 8px;
        }
        .dropzone-hint {
            color: #94a3b8;
            font-size: 13px;
        }
        .progress-container {
            display: none;
            margin-top: 15px;
        }
        .progress-bar {
            width: 100%;
            height: 24px;
            background: #e5e7eb;
            border-radius: 12px;
            overflow: hidden;
            position: relative;
        }
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            width: 0%;
            transition: width 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
            font-size: 12px;
            font-weight: 600;
        }
        .upload-status {
            text-align: center;
            margin-top: 15px;
            padding: 12px;
            font-size: 15px;
            color: #64748b;
            font-weight: 600;
            background: #f8fafc;
            border-radius: 8px;
        }
        .upload-status.success {
            background: #d1fae5;
            color: #065f46;
            border: 2px solid #10b981;
        }
        .upload-status.error {
            background: #fee2e2;
            color: #991b1b;
            border: 2px solid #ef4444;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1><img src="data:image/png;base64,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" alt="Logo" style="width:144px;height:144px;vertical-align:middle;margin-right:10px;">Slap-AI</h1>
        
        <div class="status">
            <div class="status-row">
                <span class="status-label">Status:</span>
                <span class="status-value" id="status">Loading...</span>
            </div>
            <div class="status-row">
                <span class="status-label">IP Address:</span>
                <span class="status-value" id="ip">-</span>
            </div>
            <div class="status-row" id="rssi-row" style="display: none;">
                <span class="status-label">Signal:</span>
                <span class="status-value" id="rssi">-</span>
            </div>
        </div>
        
        <div id="message" class="message"></div>
        
        <div class="section">
            <div class="section-title">🎯 Slap Threshold</div>
            <div class="slider-group">
                <div class="slider-value"><span id="threshold-value">1.0</span>g</div>
                <input type="range" id="threshold" min="0.1" max="5.0" step="0.1" value="1.0">
            </div>
            <button class="button button-primary" onclick="saveThreshold()">Save Threshold</button>
        </div>
        
        <div class="section">
            <div class="section-title">📡 WiFi Settings</div>
            <div class="input-group">
                <label>Network Name (SSID)</label>
                <input type="text" id="ssid" placeholder="Your WiFi network">
            </div>
            <div class="input-group">
                <label>Password</label>
                <input type="password" id="password" placeholder="WiFi password">
            </div>
            <button class="button button-primary" onclick="saveWiFi()">Connect to WiFi</button>
            <button class="button button-danger" onclick="resetDevice()">Factory Reset</button>
        </div>
        
        <div class="section">
            <div class="section-title">🔄 Firmware Update</div>
            <div class="dropzone" id="dropzone">
                <div class="dropzone-icon">📦</div>
                <div class="dropzone-text">Drop firmware here or click to select</div>
                <div class="dropzone-hint">Drag .bin file or click to browse</div>
            </div>
            <input type="file" id="fileInput" accept=".bin" style="display: none;">
            <div class="progress-container" id="progressContainer">
                <div class="progress-bar">
                    <div class="progress-fill" id="progressFill">0%</div>
                </div>
                <div class="upload-status" id="uploadStatus">Uploading...</div>
            </div>
        </div>
    </div>
    
    <script>
        const thresholdSlider = document.getElementById('threshold');
        const thresholdValue = document.getElementById('threshold-value');
        
        thresholdSlider.oninput = function() {
            thresholdValue.textContent = this.value;
        };
        
        function showMessage(text, isError = false) {
            const msg = document.getElementById('message');
            msg.textContent = text;
            msg.className = 'message ' + (isError ? 'error' : 'success');
            setTimeout(() => {
                msg.style.display = 'none';
            }, 5000);
        }
        
        async function loadStatus() {
            try {
                const response = await fetch('/api/status');
                const data = await response.json();
                
                document.getElementById('status').textContent = data.status;
                document.getElementById('status').className = 'status-value ' + 
                    (data.isAPMode ? 'ap' : 'connected');
                document.getElementById('ip').textContent = data.ip;
                
                if (!data.isAPMode && data.rssi !== 0) {
                    document.getElementById('rssi').textContent = data.rssi + ' dBm';
                    document.getElementById('rssi-row').style.display = 'flex';
                }
                
                thresholdSlider.value = data.threshold;
                thresholdValue.textContent = data.threshold;
                
                if (data.ssid) {
                    document.getElementById('ssid').value = data.ssid;
                }
            } catch (e) {
                console.error('Failed to load status:', e);
            }
        }
        
        async function saveThreshold() {
            const threshold = parseFloat(thresholdSlider.value);
            try {
                const response = await fetch('/api/threshold', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ threshold })
                });
                if (response.ok) {
                    showMessage('✓ Threshold saved: ' + threshold + 'g');
                } else {
                    showMessage('Failed to save threshold', true);
                }
            } catch (e) {
                showMessage('Error: ' + e.message, true);
            }
        }
        
        async function saveWiFi() {
            const ssid = document.getElementById('ssid').value;
            const password = document.getElementById('password').value;
            
            if (!ssid) {
                showMessage('Please enter a WiFi network name', true);
                return;
            }
            
            try {
                const response = await fetch('/api/wifi', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ ssid, password })
                });
                if (response.ok) {
                    showMessage('✓ Connecting to ' + ssid + '... Device will restart.');
                    setTimeout(() => {
                        window.location.reload();
                    }, 3000);
                } else {
                    showMessage('Failed to save WiFi settings', true);
                }
            } catch (e) {
                showMessage('Error: ' + e.message, true);
            }
        }
        
        async function resetDevice() {
            if (!confirm('Reset to factory defaults? This will restart in AP mode.')) {
                return;
            }
            
            try {
                const response = await fetch('/api/reset', { method: 'POST' });
                if (response.ok) {
                    showMessage('✓ Factory reset complete. Device restarting...');
                    setTimeout(() => {
                        window.location.reload();
                    }, 3000);
                } else {
                    showMessage('Failed to reset device', true);
                }
            } catch (e) {
                showMessage('Error: ' + e.message, true);
            }
        }
        
        // Drag and drop functionality
        const dropzone = document.getElementById('dropzone');
        const fileInput = document.getElementById('fileInput');
        const progressContainer = document.getElementById('progressContainer');
        const progressFill = document.getElementById('progressFill');
        const uploadStatus = document.getElementById('uploadStatus');
        
        // Prevent default drag behaviors
        ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
            dropzone.addEventListener(eventName, preventDefaults, false);
            document.body.addEventListener(eventName, preventDefaults, false);
        });
        
        function preventDefaults(e) {
            e.preventDefault();
            e.stopPropagation();
        }
        
        // Highlight drop zone when item is dragged over it
        ['dragenter', 'dragover'].forEach(eventName => {
            dropzone.addEventListener(eventName, () => {
                dropzone.classList.add('dragover');
            }, false);
        });
        
        ['dragleave', 'drop'].forEach(eventName => {
            dropzone.addEventListener(eventName, () => {
                dropzone.classList.remove('dragover');
            }, false);
        });
        
        // Handle dropped files
        dropzone.addEventListener('drop', (e) => {
            const files = e.dataTransfer.files;
            if (files.length > 0) {
                handleFile(files[0]);
            }
        }, false);
        
        // Handle click to select file
        dropzone.addEventListener('click', () => {
            fileInput.click();
        });
        
        fileInput.addEventListener('change', (e) => {
            if (e.target.files.length > 0) {
                handleFile(e.target.files[0]);
            }
        });
        
        async function handleFile(file) {
            if (!file.name.endsWith('.bin')) {
                showMessage('Please select a .bin file', true);
                return;
            }
            
            // Show progress container
            progressContainer.style.display = 'block';
            dropzone.style.display = 'none';
            
            const formData = new FormData();
            formData.append('firmware', file);
            
            try {
                const xhr = new XMLHttpRequest();
                
                // Track upload progress
                xhr.upload.addEventListener('progress', (e) => {
                    if (e.lengthComputable) {
                        const percentComplete = Math.round((e.loaded / e.total) * 100);
                        progressFill.style.width = percentComplete + '%';
                        progressFill.textContent = percentComplete + '%';
                        uploadStatus.textContent = 'Uploading... ' + Math.round(e.loaded / 1024) + ' KB / ' + Math.round(e.total / 1024) + ' KB';
                        
                        // When upload reaches 100%, show success immediately
                        // (device will reboot before we get HTTP response)
                        if (percentComplete === 100) {
                            console.log('Upload 100% - Starting countdown...');
                            progressFill.textContent = '✓ Complete';
                            uploadStatus.className = 'upload-status success';
                            
                            // Countdown from 5 to 1
                            let countdown = 5;
                            uploadStatus.textContent = `✓ Upload complete! Device restarting... Page will reload in ${countdown} seconds.`;
                            
                            const countdownInterval = setInterval(() => {
                                countdown--;
                                if (countdown > 0) {
                                    uploadStatus.textContent = `✓ Upload complete! Device restarting... Page will reload in ${countdown} second${countdown !== 1 ? 's' : ''}.`;
                                } else {
                                    uploadStatus.textContent = '✓ Reloading page now...';
                                    clearInterval(countdownInterval);
                                }
                            }, 1000);
                            
                            setTimeout(() => {
                                console.log('Reloading page...');
                                window.location.reload();
                            }, 5000);
                        }
                    }
                });
                
                xhr.addEventListener('load', () => {
                    console.log('Load event - Status:', xhr.status, 'Response:', xhr.responseText);
                    // Success handling is done in progress event at 100%
                    // This event might not fire if device reboots immediately
                });
                
                xhr.addEventListener('error', () => {
                    console.error('XHR Error event fired - upload failed');
                    // Only show error if we haven't reached 100% yet
                    if (!uploadStatus.classList.contains('success')) {
                        uploadStatus.className = 'upload-status error';
                        uploadStatus.textContent = '✗ Upload error - please try again';
                        setTimeout(() => {
                            resetUploadUI();
                        }, 3000);
                    }
                });
                
                xhr.addEventListener('loadend', () => {
                    console.log('LoadEnd event - Status:', xhr.status);
                    // Connection closed - this is normal after device reboots
                });
                
                xhr.open('POST', '/update');
                xhr.send(formData);
                
            } catch (e) {
                showMessage('Upload failed: ' + e.message, true);
                resetUploadUI();
            }
        }
        
        function resetUploadUI() {
            progressContainer.style.display = 'none';
            dropzone.style.display = 'block';
            progressFill.style.width = '0%';
            progressFill.textContent = '0%';
            uploadStatus.className = 'upload-status';
            uploadStatus.textContent = 'Uploading...';
        }
        
        // Load status on page load and refresh every 5 seconds
        loadStatus();
        setInterval(loadStatus, 5000);
    </script>
</body>
</html>
)rawliteral";
  }

 public:
  SlapWebServer(ConfigManager *cfg, SlapWiFiManager *wifi)
      : configMgr(cfg), wifiMgr(wifi) {
    server = new AsyncWebServer(80);
  }

  void begin() {
    // Serve main page
    server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
      request->send(200, "text/html", getIndexHTML());
    });

    // API: Get current status
    server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
      StaticJsonDocument<256> doc;

      doc["status"] = wifiMgr->getStatusString();
      doc["isAPMode"] = wifiMgr->isAP();
      doc["ip"] = wifiMgr->getIPAddress();
      doc["rssi"] = wifiMgr->getRSSI();
      doc["threshold"] = configMgr->getThreshold();
      doc["ssid"] = configMgr->getSSID();

      String response;
      serializeJson(doc, response);
      request->send(200, "application/json", response);
    });

    // API: Set threshold
    server->on(
        "/api/threshold", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len,
               size_t index, size_t total) {
          StaticJsonDocument<128> doc;
          DeserializationError error = deserializeJson(doc, data, len);

          if (error) {
            request->send(400, "application/json",
                          "{\"error\":\"Invalid JSON\"}");
            return;
          }

          float threshold = doc["threshold"];
          if (threshold >= 0.1 && threshold <= 5.0) {
            configMgr->setThreshold(threshold);
            configMgr->save();
            request->send(200, "application/json", "{\"success\":true}");
          } else {
            request->send(400, "application/json",
                          "{\"error\":\"Invalid threshold\"}");
          }
        });

    // API: Set WiFi credentials
    server->on(
        "/api/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len,
               size_t index, size_t total) {
          StaticJsonDocument<256> doc;
          DeserializationError error = deserializeJson(doc, data, len);

          if (error) {
            request->send(400, "application/json",
                          "{\"error\":\"Invalid JSON\"}");
            return;
          }

          const char *ssid = doc["ssid"];
          const char *password = doc["password"];

          if (ssid && strlen(ssid) > 0) {
            request->send(200, "application/json", "{\"success\":true}");

            // Delay and then switch to client mode
            delay(100);
            wifiMgr->switchToClient(ssid, password);
            delay(1000);
            ESP.restart();
          } else {
            request->send(400, "application/json",
                          "{\"error\":\"SSID required\"}");
          }
        });

    // API: Factory reset
    server->on("/api/reset", HTTP_POST, [this](AsyncWebServerRequest *request) {
      configMgr->factoryReset();
      request->send(200, "application/json", "{\"success\":true}");
      delay(100);
      ESP.restart();
    });

    // OTA Update handler
    server->on(
        "/update", HTTP_POST,
        // Handle the response after upload completes
        [](AsyncWebServerRequest *request) {
          bool success = !Update.hasError();

          Serial.printf("OTA Upload complete. Success: %d\n", success);

          // Send response to browser
          request->send(200, "text/plain", success ? "OK" : "FAIL");

          if (success) {
            Serial.println("OTA Update Success! Rebooting in 500ms...");
            // Delay to ensure response is sent before reboot
            delay(500);
            ESP.restart();
          } else {
            Serial.println("OTA Update Failed!");
          }
        },
        // Handle the upload data
        [](AsyncWebServerRequest *request, String filename, size_t index,
           uint8_t *data, size_t len, bool final) {
          if (!index) {
            Serial.printf("OTA Update Start: %s\n", filename.c_str());

            // Determine update type based on filename
            int cmd = (filename.indexOf("spiffs") > -1 ||
                       filename.indexOf("littlefs") > -1)
                          ? U_SPIFFS
                          : U_FLASH;

            if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
              Update.printError(Serial);
            }
          }

          // Write data chunk
          if (len) {
            if (Update.write(data, len) != len) {
              Update.printError(Serial);
            }
          }

          if (final) {
            if (Update.end(true)) {
              Serial.printf("OTA Update Success: %u bytes\n", index + len);
            } else {
              Update.printError(Serial);
            }
          }
        });

    // Start server
    server->begin();
    Serial.println("Web server started on port 80");
  }

  void stop() { server->end(); }
};

#endif  // WEBSERVER_H
