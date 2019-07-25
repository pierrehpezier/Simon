#define YELLOW_LIGHT  PIN_A0
#define RED_LIGHT     PIN_A1
#define BLUE_LIGHT    PIN_A2
#define GREEN_LIGHT   PIN_A3

#define YELLOW_IN     2
#define RED_IN        3
#define BLUE_IN       4
#define GREEN_IN      5

#define YELLOW        0
#define RED           1
#define BLUE          2
#define GREEN         3
#define STATE_IDLE    4

class simon {
  public:
    simon();
    void lightOn(uint16_t light) {setLight(light);}
    void lightOff(uint16_t light) {setLight(light, LOW);}
    void lightsOn(void);
    void lightsOff(void);
    void blinkLights(uint16_t iterations=10, uint16_t delayiter=150);
    void createNewGame(void);
    uint16_t getInput(void);
    bool nextRound(void);
    bool hasWon(void);
  private:
    uint16_t new_game[20];
    uint16_t gamecounter;
    uint16_t output[4] = {YELLOW_LIGHT, RED_LIGHT, BLUE_LIGHT, GREEN_LIGHT};
    uint16_t input[4] = {YELLOW_IN, RED_IN, BLUE_IN, GREEN_IN};
    void setLight(uint16_t light, uint16_t currstatus=HIGH) {digitalWrite(light, currstatus);}
    void play_sequence(void);
    bool check_sequence(void);
};

simon::simon()
{
   for(uint16_t i=0; i<4; i++) {
      pinMode(output[i], OUTPUT);
      pinMode(input[i], INPUT);
   }
   lightsOff();
}
bool simon::hasWon(void)
{
  return gamecounter == sizeof(new_game);
}
void simon::lightsOn(void)
{
  for(uint16_t i=0; i<4; i++) lightOn(output[i]);
}
void simon::lightsOff(void)
{
   for(uint16_t i=0; i<4; i++) lightOff(output[i]); 
}
void simon::createNewGame(void)
{
  randomSeed(analogRead(0));
  for(unsigned short i=0; i<sizeof(new_game); i++) {
      new_game[i] = random(0, 4);
  }
  gamecounter = 1;
}
void simon::blinkLights(uint16_t iterations, uint16_t delayiter)
{
  for(uint16_t i=0; i<iterations; i++) {
    lightsOn();
    delay(delayiter);
    lightsOff();
  }
}
uint16_t simon::getInput(void)
{
  while(true) {
    for(unsigned short i=0; i<sizeof(input); i++) {
        if(digitalRead(input[i]) == HIGH)
          return input[i];
    }
  }
}
void simon::play_sequence(void)
{
  for(uint16_t i=0; i<gamecounter; i++) {
    lightOn(output[new_game[i]]);
    delay(500);
    lightsOff();
  }
}
bool simon::check_sequence(void)
{
  for(uint16_t i=0; i<gamecounter; i++) {
    if(getInput() != input[new_game[i]]) return false;
  }
  return true;
}
bool simon::nextRound(void)
{
  play_sequence();
  bool retval = check_sequence();
  gamecounter++;
  return retval;
}

simon *game;

void setup() {
  game = new simon();
}

void loop() {
  game->createNewGame();
  game->getInput();//Wait for user interraction
  while(game->nextRound()) {
    game->lightsOff();
    if(game->hasWon()) {
      game->blinkLights();
    }
  }
  game->lightsOff();
}

