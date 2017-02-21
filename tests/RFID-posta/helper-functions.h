

#ifndef HEADER_HELPERFUNC
  #define HEADER_HELPERFUNC
  

  void handleNotFound(void);
  
  
  
  // read UDP incoming packages
  
  void UDPRead(void);
  
  // connect to UDP – returns true if successful or false if not
  boolean connectUDP(void);
  
  // connect to wifi – returns true if successful or false if not
  boolean connectWifi(void);

   
#endif
