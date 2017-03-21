# Guardian2Chaperone

## Purpose

This utility transfers the Oculus Room Setup data to SteamVR for Oculus Rift. It moves the SteamVR floor to match the Oculus floor, the SteamVR play area to Oculus play area, and makes the Chaperone bounds match the Oculus bounds [and makes the SteamVR bounds invisible]

## Usage

Set up Oculus Room Setup, then run this utility. You can run this utility instead of running SteamVR Room Setup

## Notes

* This code makes a lot of assumptions that I believe are valid, but I am not 100% certain if they are valid for all configurations, and if they will be valid in the future. 

* This utility has been observed to cause SteamVR to freeze if run while SteamVR is running. Just close SteamVR via the Universal Menu and start it again.

* This turns the SteamVR bounds invisible. Keep Oculus Guardian on, or make the SteamVR bounds visible in the SteamVR customization menu. The SteamVR bounds will match Oculus bounds, but might look ugly.

* The Oculus play space may be smaller than SteamVR's usual minimum play space. The utility will set the SteamVR play space to match Oculus, but the utility will give no notice that this has occurred, and I cannot predict how SteamVR applications will react.

## Credits

* Oculus's GuardianSystemDemo used as a starting point, but almost none of that code remains.

* OpenVR-Advanced for helping me understand the OpenVR API functions required.
