The babel is an Epitech project who's goal is to create a VoIP protocol and implement it in c++ using the following libraries:  
  
Qt (client side)  
Boost (server/client side)  
BoostAsio (server side)  
PortAudio (client side)  
Opus (client side)  
sqlite3 (server side)  
  
The project should be cross platform (windows/linux) and consists of a server and a client. The server connects clients between them and handle messages/commands,   but the voice communication should be done in p2p using udp between two clients.  
  
<img src="https://raw.github.com/L0rentz/Babel/master/examples/example.gif"/>  
  
Usage :  
  
You will need conan first, good luck.  
  
After that use ./build.sh  
  
Then copy paste this in your terminal :  
export ALSA_CONFIG_DIR=/usr/share/alsa  
export ALSA_CONFIGDIR=~/.conan/data/libalsa/1.1.9//_/package/d48130e0dd76369b1338deb3b2372c5a649f9f2d/res/alsa  
  
Now you can go in the folder build and from this folder start the server located in bin first and then any amount of clients.  
