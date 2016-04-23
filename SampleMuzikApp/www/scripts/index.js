// For an introduction to the Blank template, see the following documentation:
// http://go.microsoft.com/fwlink/?LinkID=397704
// To debug code on page load in Ripple or on Android devices/emulators: launch your app, set breakpoints, 
// and then run "window.location.reload()" in the JavaScript Console.
(function () {
    "use strict";

    document.addEventListener( 'deviceready', onDeviceReady.bind( this ), false );

    function onDeviceReady() {
        // Handle the Cordova pause and resume events
        document.addEventListener( 'pause', onPause.bind( this ), false );
        document.addEventListener( 'resume', onResume.bind( this ), false );
        
        $("#leftArrow").hide();
        $("#rightArrow").hide();
        $("#upArrow").hide();
        $("#downArrow").hide();

        // Opening a Bluetooth connection with the Muzik Convertibles
        muzik.startServer();

        muzik.registerForConnectionState(function onStateChanged(state) {
            if (state == muzik.CONNECTION_STATE.CONNECTED) {
                $('#dashboard').show();
                $('#statusMessageContainer').hide();

                muzik.registerForAllGestures(function onGesture(gesture) {
                    stripIndicatorsFromButtons();

                    if (gesture == muzik.GESTURE.BUTTON_FORWARD) {
                        $("#rightIndicator").addClass("led-green-on");
                    }
                    if (gesture == muzik.GESTURE.BUTTON_BACK) {
                        $("#leftIndicator").addClass("led-green-on");
                    }
                    if (gesture == muzik.GESTURE.BUTTON_UP) {
                        $("#topIndicator").addClass("led-green-on");
                    }
                    if (gesture == muzik.GESTURE.BUTTON_DOWN) {
                        $("#bottomIndicator").addClass("led-green-on");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_BACK)
                    {
                        $("#leftArrow").show();
                        $("#leftArrow").addClass("flyLeft");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_FORWARD) {
                        $("#rightArrow").show();
                        $("#rightArrow").addClass("flyRight");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_UP) {
                        $("#upArrow").show();
                        $("#upArrow").addClass("flyUp");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_FAST_UP) {
                        $("#upArrow").show();
                        $("#upArrow").addClass("flyFastUp");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_DOWN) {
                        $("#downArrow").show();
                        $("#downArrow").addClass("flyDown");
                    }
                    if (gesture == muzik.GESTURE.SWIPE_FAST_DOWN) {
                        $("#downArrow").show();
                        $("#downArrow").addClass("flyFastDown");
                    }

                    setTimeout(stripIndicatorsFromButtons, 1500);
                }, true);

                muzik.registerForMotions(function onMotion(motion) {
                    $(".headMotions").removeClass("led-blue-on");
                    if (motion == muzik.MOTION.MOVING) {
                        $("#movingIndicator").addClass("led-blue-on");
                    }
                    if (motion == muzik.MOTION.VERTICAL) {
                        $("#verticalIndicator").addClass("led-blue-on");
                    }
                    if (motion == muzik.MOTION.STILL) {
                        $("#stillIndicator").addClass("led-blue-on");
                    }
                    if (motion == muzik.MOTION.WALKING) {
                        $("#walkingIndicator").addClass("led-blue-on");
                    }
                    if (motion == muzik.MOTION.BOBBING) {
                        $("#bobbingIndicator").addClass("led-blue-on");
                    }
                    if (motion == muzik.MOTION.OTHER) {
                        $("#otherIndicator").addClass("led-blue-on");
                    }
                }, muzik.MOTION.MOVING, muzik.MOTION.VERTICAL, muzik.MOTION.STILL, muzik.MOTION.WALKING, muzik.MOTION.BOBBING, muzik.MOTION.OTHER);

                muzik.registerForAccelerometerDataStream(function onAccelerometerSample(x, y, z, norm, fwdAngle, sideAngle) {
                    $("#accelX").html(Math.round(x * 10000) / 10000);
                    $("#accelY").html(Math.round(y * 10000) / 10000);
                    $("#accelZ").html(Math.round(z * 10000) / 10000);
                    $("#accelNorm").html(Math.round(norm * 10000) / 10000);
                    $("#accelFwdAngle").html(Math.round(fwdAngle * 10000) / 10000);
                    $("#accelSideAngle").html(Math.round(sideAngle * 10000) / 10000);
                });

                pollDeviceMetaData();
            }
            else if (state == muzik.CONNECTION_STATE.HEADPHONES_NOT_CONNECTED) {
                $('#dashboard').hide();
                $('#statusMessageContainer').show();
                $('#statusMessage').html("Headphones not Connected");
            }
            else if (state == muzik.CONNECTION_STATE.INTERNAL_ERROR) {
                $('#dashboard').hide();
                $('#statusMessageContainer').show();
                $('#statusMessage').html("Internal Error");
            }
            else if (state == muzik.CONNECTION_STATE.NO_BLUETOOTH_SUPPORT) {
                $('#dashboard').hide();
                $('#statusMessageContainer').show();
                $('#statusMessage').html("No Bluetooth Support");
            }
            else if (state == muzik.CONNECTION_STATE.BLUETOOTH_NOT_ENABLED) {
                $('#dashboard').hide();
                $('#statusMessageContainer').show();
                $('#statusMessage').html("Bluetooth not Enabled");
            }
        });

        
    };

    function stripIndicatorsFromButtons()
    {
        $('.swipeArrows').removeClass("flyLeft");
        $('.swipeArrows').removeClass("flyRight");
        $('.swipeArrows').removeClass("flyUp");
        $('.swipeArrows').removeClass("flyFastUp");
        $('.swipeArrows').removeClass("flyDown");
        $('.swipeArrows').removeClass("flyFastDown");

        $('.swipeArrows').hide();

        $(".headsetCup div").removeClass("led-green-on");
    }

    function pollDeviceMetaData()
    {
        muzik.getChargeStatus(function onChargingStatus(isCharging) {
            if (isCharging) {
                $("#chargingIcon").show();
            }
            else {
                $("#chargingIcon").hide();
            }
        });

        muzik.getBatteryLevel(function onBatteryLevelReceived(batteryLevel) {
            $("#batteryLevel").html(batteryLevel + "%");
            $("#batteryLevel").css('width', batteryLevel + "%");
            $("#batteryLevel").attr('aria-valuenow', batteryLevel);
        });

        muzik.getSerialNumber(function onSerialNum(serialNum) { $("#serialNumber").html(serialNum); });
        muzik.getFirmwareVersion(function onFwVersion(version) { $("#firmwareVersion").html(version); });
        muzik.getHardwareVersion(function onHwVersion(version) { $("#hardwareVersion").html(version); });
        $("#libraryVersion").html(muzik.getLibraryVersion());

        setTimeout(pollDeviceMetaData, 5000);
    }

    function onPause() {
        // TODO: This application has been suspended. Save application state here.
    };

    function onResume() {
        // TODO: This application has been reactivated. Restore application state here.
    };
} )();