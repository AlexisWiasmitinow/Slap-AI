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
