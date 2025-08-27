# MatrixClock
An RGB LED Matrix Clock and more!

### Setup

## Install ESP-IDF
In the repository directory (`MatrixClock/`)
```
git clone https://github.com/espressif/esp-idf.git
cd esp-idf
./install.ps1
./export.ps1
```

## Configure ESP-IDF
```
idf.py set-target esp32s3
idf.py reconfigure
```

### Build and Deployment

## Build Project
```
idf.py build
```

## Flash to ESP32
```
idf.py -p PORT flash
```

## Open Serial Monitor
```
idf.py monitor
```