
#ifndef HEADER_SIMON
  #define HEADER_SIMON
 
  
  void setLEDs();
  
  void attractMode(void)
  
  boolean play_memory(void);
  
  void playMoves(void);
  
  void add_to_moves(void);
  
  byte wait_for_button(void);
  
  byte checkButton(void);
  
  void toner(byte which, int buzz_length_ms);
  
  void buzz_sound(int buzz_length_ms, int buzz_delay_us);
  
  
  void play_winner(void);
  
  void winner_sound(void);
  
  void play_loser(void);
  
  
  // luces locas
  
  void rayoAvanza();
  
  void updateTurbina();
  
  void rayoPega();
  
  // handle incoming
  
  void checkIncoming();
  
  void handleSillaOn();
  
  void handleSillaOff();
  
  void handleTrueno();

#endif
