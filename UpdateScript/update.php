<?PHP

header('Content-type: text/plain; charset=utf8', true);

function check_header($name, $value = false) {
    if(!isset($_SERVER[$name])) {
        return false;
    }
    if($value && $_SERVER[$name] != $value) {
        return false;
    }

    return true;
}

function sendFile($path) {
    header($_SERVER["SERVER_PROTOCOL"].' 200 OK', true, 200);
    header('Content-Type: application/octet-stream', true);
    header('Content-Disposition: attachment; filename='.basename($path));
    header('Content-Length: '.filesize($path), true);
    header('x-MD5: '.md5_file($path), true);
    readfile($path);
}

if(!check_header('HTTP_USER_AGENT', 'ESP8266-http-Update')) {
    header($_SERVER["SERVER_PROTOCOL"].' 403 Forbidden', true, 403);
    echo "only for ESP8266 updater!\n";
    exit();
}

if(
    !check_header('HTTP_X_ESP8266_STA_MAC') ||
    !check_header('HTTP_X_ESP8266_AP_MAC') ||
    !check_header('HTTP_X_ESP8266_FREE_SPACE') ||
    !check_header('HTTP_X_ESP8266_SKETCH_SIZE') ||
    !check_header('HTTP_X_ESP8266_SKETCH_MD5') ||
    !check_header('HTTP_X_ESP8266_CHIP_SIZE') ||
    !check_header('HTTP_X_ESP8266_SDK_VERSION')
) {
    header($_SERVER["SERVER_PROTOCOL"].' 403 Forbidden', true, 403);
    echo "only for ESP8266 updater! (header)\n";
    exit();
}

$version_db = array(
    "control"   => "test_v1.1",
    "door"      => "test_v1.1"
);

$type_db = array(
    "A0:20:A6:16:9A:33" => "control",       //NodeMCU
    "A0:20:A6:17:BA:98" => "control",       //NodeMCU
    "A0:20:A6:14:82:BA" => "door"           //ESP8266 Mod
);

if(!isset($type_db[$_SERVER['HTTP_X_ESP8266_STA_MAC']])) {
    header($_SERVER["SERVER_PROTOCOL"].' 500 ESP MAC not configured for updates', true, 500);
}

$boardType   = $type_db[$_SERVER['HTTP_X_ESP8266_STA_MAC']];
$version     = $version_db[$boardType];
$localBinary = "../bin/".$boardType."/".$version.".bin";
$localSpiffs = "../spiffs_bin/".$boardType."/".$version.".spiffs.bin";

if($_SERVER['HTTP_X_ESP8266_MODE'] == "sketch"){
    // Check if version has been set and does not match, if not, check if
    // MD5 hash between local binary and ESP8266 binary do not match if not.
    // then no update has been found.
    if((!check_header('HTTP_X_ESP8266_SDK_VERSION') && $version != $_SERVER['HTTP_X_ESP8266_VERSION'])
            || $_SERVER["HTTP_X_ESP8266_SKETCH_MD5"] != md5_file($localBinary)) {
        sendFile($localBinary);
    } else {
        header($_SERVER["SERVER_PROTOCOL"].' 304 Not Modified', true, 304);
    }
 } else if($_SERVER['HTTP_X_ESP8266_MODE'] == "spiffs"){
     // Check if version has been set and does not match, if not, check if
     // Check if spiffs.bin exists
     if((!check_header('HTTP_X_ESP8266_SDK_VERSION') && $version != $_SERVER['HTTP_X_ESP8266_VERSION'])
            || file_exists($localSpiffs)) {
         sendFile($localSpiffs);
     } else {
         header($_SERVER["SERVER_PROTOCOL"].' 304 Not Modified OR Selected', true, 304);
     }
 }else{
     header($_SERVER["SERVER_PROTOCOL"].' 403 Forbidden', true, 403);
     echo "only for ESP8266 updater! (header)\n";
     exit();
 }

//header($_SERVER["SERVER_PROTOCOL"].' 500 no version for ESP MAC', true, 500);