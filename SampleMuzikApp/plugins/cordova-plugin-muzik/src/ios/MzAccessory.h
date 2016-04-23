//
//  MzAccessory.h
//  MzAccessory
//
//  Created by greenhouse on 2/6/15.
//  Copyright (c) 2015 furthr llc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@protocol MzAccessoryDelegate <NSObject>
@optional
/*
 Possible dictionary keys to map your delegate update return:
     {kUpdateAccAtt : [NSNumber boolValue]}                 // NOT requestable
     {kUpdateBatteryLevel : [NSNumber integerValue]}        // requestable
     {kUpdateChargeStatus : [NSNumber boolValue]}           // requestable
     {kUpdateBTLocalName : NSString}                        // requestable
     {kUpdateDevConnected : [NSNumber boolValue]}           // requestable
     {kUpdateAccelData : (NSDictionary){kUpdateAccelDataX : (NSNumber shortValue),       // requestable
                                         kUpdateAccelDataY : (NSNumber shortValue),
                                         kUpdateAccelDataZ : (NSNumber shortValue)}}
     {kUpdateMetaDataDict : (NSDictionary){kUpdateMetaDataSoft : (NSString)softversion,  // requestable
                                             kUpdateMetaDataHard : (NSString)hardversion,
                                             kUpdateMetaDataMan : (NSString)manufacturer}}

- (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
*/

- (void) WasAttached:(BOOL)attached;
- (void) WasConnected:(BOOL)connected;
- (void) GestureReceived:(NSInteger) gestureIndex;
- (void) BatteryLevelReceived:(NSInteger)batteryPercentage;
- (void) AccelerometerDataReceived:(NSDictionary*)accelData;
- (void) AutoPlaySettingReceived:(BOOL)autoPlay;
- (void) ChargeStatusReceived:(BOOL)charging;
- (void) BTLocalNameReceived:(NSString*)localName;
- (void) MspVersionReceived:(NSString*)mspVersion;
- (void) DeviceMetaDataReceived:(NSDictionary*)metaData;
- (void) TrackInfoReceived:(NSDictionary*)trackInfo;


/*
 Possible action keys to receive:
 (NOTE: taps can be suggested as a good firm press or 1 to 2 second hold)
    kGesture3               // tap 3 o'clock button
    kGesture12              // tap 12 o'clock button
    kGesture9               // tap 9 o'clock button
    kGesture6               // tap 6 o'clock button

    kGesturePass12          // swipe up
    kGesturePassF12         // swipe up fast
    kGesturePass6           // swipe down
    kGesturePassF6          // swipe down fast
    kGesturePass9           // swipe back
    kGesturePass3           // swipe forward
    kGesturePassTap         // tap with 2 fingers
    kGesturePassTapHold     // tap and hold 2 fingers for 3 seconds
 */
- (void)receivedGestureForActionKey:(NSUInteger)key;

/*
 Possible authorization dictionary responses:
    {kAuthStatus : [NSNumber numberWithBool:isAuthorized]}  // requestable
    {kAuthStatus : [NSNumber numberWithBool:isAuthorized],  // NOT requestable
      kAuthError : (NSString)err}
 */
- (void)receivedAuthorization:(BOOL)isAuthorized withError:(NSString*)error;

@end


//@interface MzAccessory : NSObject <NptAccessoryDelegate>
@interface MzAccessory : NSObject
@property (nonatomic, strong) NSString *singletonTestString;

#pragma mark mzaccessory interface methods

/**
 * Add a delegate object to receive 'gesture' and 'update' events
 * @author greenhouse
 *
 * @param id<MzAccessoryDelegate> delegate passed to received MzAccessory notifications
 * @note must conform to MzAccessoryDelegate protocol
 * @note SDK 2.0 currently only supports registering one delegate object at a time
 * @return nil
 */
- (void)addUpdateDelegate:(id<MzAccessoryDelegate>)delegate;

/**
 * Initializes the call back engine for receiving responses to requests
 *  - opens connection between the app and the headphones
 *  - enables press & hold gestures and swipe gestures
 * @author greenhouse
 *
 * @prints device connection status, device software/hardware/library version, device manufacturer
 * @param nil
 * @return nil
 */
- (void)initializeCallbackEngine;

/**
 * Authorizes the API for use
 *  This function will need to be called prior to every
 *      first utilization of MzAccessory properties or functions
 * @author greenhouse
 *
 * @param key - key received after registering for access with Muzik
 * @return nil
 */
- (void)authorizeDevWithKey:(NSString *)key;

/**
 * Remove the update delegate object that receives 'gesture' and 'update' events
 * @author greenhouse
 *
 * @param id<MzAccessoryDelegate> delegate passed to received MzAccessory notifications
 *          note: must conform to MzAccessoryDelegate protocol
 * @return nil
 */
- (void)removeUpdateDelegate;

/**
 * Check is API is authorized for use
 * @author greenhouse
 *
 * @param nil
 * @return YES if authorized;
 * @return NO if not && invokes callback receivedAuthResponseDict: with kAuthStatus
 * @callback if NO only: - (void)receivedAuthResponseDict:(NSDictionary*)dict;
 *                      dict : {kAuthStatus: [NSNumber boolvalue]}
 */
- (BOOL)checkIsAuthorized;

/**
 * Sets a custom created gesture action key to map to a specific gesture type.
 *  You can use this key to trigger an event in your application.
 *  Action key bindings to gesture type are stored with NSUserDefaults
 *   Which means:
 *      - gesture actions will be saved on app termination and phone reset
 *      - gesture actions will be lost on app re-install
 *      - using different headphones on the same phone with the same application
 *          will result in the headphones performing the same actions for gesture types
 * @author greenhouse
 *
 * @param key - a custom action key that you create to map to a gesture type
 * @param type - predefined gesture type
 * @return nil
 */
- (void)setGestureActionKey:(NSUInteger)key forGestureType:(NSString*)type;

/**
 * Gets a previously created gesture action key that was mapped to a specific gesture type
 * @author greenhouse
 *
 * @param type - predefined gesture type that the action key you want is mapped to
 * @return NSUInteger - saved action key of the gesture type
 */
- (NSUInteger)getGestureActionKeyForType:(NSString*)type;
//- (void)setGestureCallback:(void(^)(NSDictionary *info))fn_callback forActionType:(NSString*)type;

#pragma mark delegate requests
/**
 * Makes a request for the current dev API authorization status
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedAuthResponseDict:(NSDictionary*)dict;
 *  dict : {kAuthStatus: [NSNumber boolvalue]}
 */
- (void)requestIsDevAuthorized;

/**
 * Makes a request to the device for its current battery level as a precent
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateBatteryLevel : [NSNumber integerValue]}
 */
- (void)requestBatteryLevelPercent;

/**
 * Makes a request to the device for its current charge status
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateChargeStatus : [NSNumber boolValue]}
 */
- (void)requestDeviceChargeStatus;

/**
 * Makes a request to the device for its local bt broadcast name
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateBTLocalName : [NSNumber stringValue]}
 */
- (void)requestDeviceBlueToothLocalName;

/**
 * Makes a request to the device to check if its connected
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateDevConnected : [NSNumber boolValue]}
 */
- (void)requestIsDeviceConnected;

/**
 * Makes a request to the device for its current meta data
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateMetaDataDict : (NSDictionary) {kUpdateMetaDataSoft : (NSString)softversion,
 *                                                kUpdateMetaDataHard : (NSString)hardversion,
 *                                                kUpdateMetaDataMan : (NSString)manufacturer}}
 */
- (void)requestDeviceMetaData;

/**
 * Makes a request to the device for its accelerometer data
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param nil
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateAccelData : (NSDictionary){kUpdateAccelDataX : (NSNumber shortValue),
 *                                            kUpdateAccelDataY : (NSNumber shortValue),
 *                                            kUpdateAccelDataZ : (NSNumber shortValue)}}
 */
- (void)requestDeviceAccelerometerData;

/**
 * Set accelerometer continuous receive buffer on/off
 * @author greenhouse
 *
 * @requires registered update delegate
 * @param BOOL YES - turn buffer on; NO - turn buffer off
 * @callback - (void)receivedUpdateWithDictionary:(NSDictionary*)dict;
 *  dict : {kUpdateAccelData : (NSDictionary){kUpdateAccelDataX : (NSNumber shortValue),
 *                                            kUpdateAccelDataY : (NSNumber shortValue),
 *                                            kUpdateAccelDataZ : (NSNumber shortValue)}}
 */
- (void)setAccelerometerDataReceiveBuffer:(BOOL)on;

- (void)requestTrackInfo;

@end



