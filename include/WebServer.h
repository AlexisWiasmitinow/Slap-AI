#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <Update.h>

// Define HTTP methods if not already defined
#ifndef HTTP_GET
typedef enum {
    HTTP_GET     = 0b00000001,
    HTTP_POST    = 0b00000010,
    HTTP_DELETE  = 0b00000100,
    HTTP_PUT     = 0b00001000,
    HTTP_PATCH   = 0b00010000,
    HTTP_HEAD    = 0b00100000,
    HTTP_OPTIONS = 0b01000000,
    HTTP_ANY     = 0b01111111,
} WebRequestMethod;
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "WiFiManager.h"

class SlapWebServer {
private:
    AsyncWebServer* server;
    ConfigManager* configMgr;
    SlapWiFiManager* wifiMgr;
    
    // HTML page with embedded CSS and JavaScript
    const char* getIndexHTML() {
        return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Slap-AI Settings</title>
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
        <h1>⚡ Slap-AI</h1>
        <div class="subtitle">Motion Detection Configuration</div>
        
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
    SlapWebServer(ConfigManager* cfg, SlapWiFiManager* wifi) 
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
        server->on("/api/threshold", HTTP_POST, [](AsyncWebServerRequest *request) {}, 
            NULL, 
            [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
                StaticJsonDocument<128> doc;
                DeserializationError error = deserializeJson(doc, data, len);
                
                if (error) {
                    request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
                    return;
                }
                
                float threshold = doc["threshold"];
                if (threshold >= 0.1 && threshold <= 5.0) {
                    configMgr->setThreshold(threshold);
                    configMgr->save();
                    request->send(200, "application/json", "{\"success\":true}");
                } else {
                    request->send(400, "application/json", "{\"error\":\"Invalid threshold\"}");
                }
            }
        );
        
        // API: Set WiFi credentials
        server->on("/api/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {}, 
            NULL,
            [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
                StaticJsonDocument<256> doc;
                DeserializationError error = deserializeJson(doc, data, len);
                
                if (error) {
                    request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
                    return;
                }
                
                const char* ssid = doc["ssid"];
                const char* password = doc["password"];
                
                if (ssid && strlen(ssid) > 0) {
                    request->send(200, "application/json", "{\"success\":true}");
                    
                    // Delay and then switch to client mode
                    delay(100);
                    wifiMgr->switchToClient(ssid, password);
                    delay(1000);
                    ESP.restart();
                } else {
                    request->send(400, "application/json", "{\"error\":\"SSID required\"}");
                }
            }
        );
        
        // API: Factory reset
        server->on("/api/reset", HTTP_POST, [this](AsyncWebServerRequest *request) {
            configMgr->factoryReset();
            request->send(200, "application/json", "{\"success\":true}");
            delay(100);
            ESP.restart();
        });
        
        // OTA Update handler
        server->on("/update", HTTP_POST, 
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
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                if (!index) {
                    Serial.printf("OTA Update Start: %s\n", filename.c_str());
                    
                    // Determine update type based on filename
                    int cmd = (filename.indexOf("spiffs") > -1 || filename.indexOf("littlefs") > -1) 
                        ? U_SPIFFS : U_FLASH;
                    
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
            }
        );
        
        // Start server
        server->begin();
        Serial.println("Web server started on port 80");
    }
    
    void stop() {
        server->end();
    }
};

#endif // WEBSERVER_H

