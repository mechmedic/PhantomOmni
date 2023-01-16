# PhantomOmni
Simple Qt GUI program in Windows 10 for reading from two FireWire Phantom Omni

Installing Phantom Omni on Windows 10
1. Install 1394 card to PC.  Need legacy firewire driver which was only included up to windows 7. The driver should be downloaded from Microsoft. Follow the instructions below

https://kyle.machul.is/2016/10/03/using-a-firewire-phantom-omni-on-windows-8-10/
https://support.microsoft.com/en-us/topic/firewire-port-based-device-does-not-work-correctly-in-windows-8-1-or-windows-8-2242eed9-e8e2-00eb-c854-9335ea06c5fe
https://www.studio1productions.com/Articles/Firewire-1.htm

2. Download phantom omni driver and opens haptics sdk 3.5 from 3D systems

https://support.3dsystems.com/s/article/Haptic-Device-Drivers-for-OpenHaptics?language=en_US
https://support.3dsystems.com/s/article/OpenHaptics-for-Windows-Developer-Edition-v35?language=en_US

3. Using Haptic Device library requires 'msvcr120.dll'. Install "Visual C++ Redistributable Package for Visual Studio 2013" for this. 

https://blog.naver.com/eternalguard/221477718123
https://www.microsoft.com/ko-KR/download/details.aspx?id=40784

4. Install Visual Studio 2015 Community or build tool for MSVC 2019 (version 16.11) if you are developing using another IDE. They can be downloaded from 
https://visualstudio.microsoft.com/ with MS account. 
