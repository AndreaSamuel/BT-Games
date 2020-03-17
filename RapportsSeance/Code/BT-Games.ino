#include <SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#define TFT_DC 9              
#define TFT_CS 7             
#define TFT_RST 8             
#define TFT_MISO 12           
#define TFT_MOSI 6          
#define TFT_CLK 13    
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

String rep = "";
String Fin;
int mode;
int r1;
int r2;

String couleur[4] = {"rouge","vert","bleue","jaune"};
String GFXcouleur[4] = {"ILI9341_RED","ILI9341_GREEN","ILI9341_BLUE","ILI9341_YELLOW"};
String Couleur[4] = {"Pique","Trèfle","Coeur","Carreau"};
String Valeur[13] = {"AS","2","3","4","5","6","7","8","9","10","Valet","Dame","Roi"};
int Rec[6];

//TicTacToe
int L3[3] = {0,0,0};
int L2[3] = {0,0,0};
int L1[3] = {0,0,0};

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  delay(500);
  Serial.println("~~~~~~~~~~~~~~~~");
  BlueT.begin(9600);
  delay(500);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}



void loop(){ //Menu Principal
//  config();
  delay(25);
  rep = lireBT();
  
  if(rep!=""){
    Serial.print("Commande recue : ");
    Serial.println(rep);
    
    if(rep=="M"){
      BlueT.print("2");
      Serial.println("Mode Multi");
      mode = 2;      
    }
    else if(rep=="S"){
      BlueT.print("1");
      Serial.println("Mode Solo");
      mode = 1;
    }
    else if(rep=="L"){
      Serial.println("LeaderBoard");
      mode = 1;
    }
    else if(rep=="1"){ //Lance Reflex
     BlueT.print("R");
     ReflexCLI(mode);
    }
    else if(rep=="2"){ //Lance Simon
      BlueT.print("S");
      SimonCLI(mode);
    }
    else if(rep=="3"){ //Lance Mastermind
      BlueT.print("M");
      MastermindCLI(mode);
    }
    else if(rep=="4"){ //Lance BlackJack
      BlueT.print("B");
      BlackJackCLI(mode);
    }
    else if(rep=="5"){ //Lance Yams
      BlueT.print("Y");
      YamsCLI(mode);
    }
    else if(rep=="6"){ //Lance Pendu
      PenduCLI(mode);
    }
    else if(rep=="7"){ //Lance TicTacToe
      BlueT.print("T");
      TicTacToe(mode);
    }
    else{
      Serial.println("Mauvaise commande");
    }
  rep=""; //reset commande
  }
}

String lireBT() {
  int n;
  char cBT;
  String rBT="";
  String rBT2="";
  if (BlueT.available()){ 
    if ((char)BlueT.read()=='X'){
      while (true) {
        cBT = (char)BlueT.read();
        if (cBT=='X') {
          break;
        }
        rBT+=cBT;
      }
    }
  }
  return rBT;
}

void viderBT() {
  while (BlueT.available()) {
  BlueT.read();
  }
}

void ReflexCLI(int mode){
  uint8_t rotation=1;
  tft.setRotation(rotation);
  int Score = 0;
  String pos[4] = {"A","B","C","D"};
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(70, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("PARTIE EN COURS");
  
  for (int i = 0; i<4 ;i++) {
    viderBT();
    
    r1 = random(4);
    r2 = random(6);
    int tps = 0;


    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.print(i+1);
    Serial.println("/4");
    delay(r2*1000);
    Serial.print("La lettre est :");
    Serial.println(pos[r1]);
    /*
    if(pos[r1]=="A"){
      tft.setCursor(70, 0);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(2);
      tft.println("A");
      }
    if(pos[r1]=="B"){
      tft.setCursor(70, 0);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(2);
      tft.println("B");
      }
    if(pos[r1]=="C"){
      tft.setCursor(70, 0);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(2);
      tft.println("C");
      }
    if(pos[r1]=="D"){
      tft.setCursor(70, 0);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(2);
      tft.println("D");
      }
      */
    tft.setCursor(150, 100);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(8);
    tft.println(pos[r1]);

    delay(100);
    long timer = millis();
    
    while (true) {
      rep = "";
      tps = (millis()-timer);
      rep = lireBT();
      
      if (rep==""){
        if (tps > 7000) {
          Serial.println("Temps écoulé");
          break;
        }
      }
      else if((rep=="A")or(rep=="B")or(rep=="C")or(rep=="D")){
        String tpsstr = String(tps);
        if (rep == pos[r1]){
            tft.setCursor(20, 200);
            tft.setTextColor(ILI9341_WHITE);  
            tft.setTextSize(2);
            tft.println("BONNE REPONSE ("+tpsstr+" ms)");
            Serial.println("Bonne Réponse !!");
            Serial.print("Temps de réponse :");
            Serial.print(tps);
            Serial.println(" ms");
            Score += 7000 - tps;
            delay(2000);
            tft.fillRect(0, 200, 320, 60, ILI9341_BLACK);
            break;
        }
        else{
          tft.setCursor(20, 200);
          tft.setTextColor(ILI9341_WHITE);  
          tft.setTextSize(2);
          tft.println("MAUVAISE REPONSE ("+tpsstr+" ms)");
          Serial.println(rep + " est une mauvaise réponse !!");
          Serial.print("Temps de réponse :");
          Serial.print(tps);
          Serial.println(" ms");
          delay(2000);
          tft.fillRect(0, 200, 320, 60, ILI9341_BLACK);
          break;
        }
      }
      delay(25);
    }
    tft.fillRect(150, 100, 80, 80, ILI9341_BLACK);
  }
  String Scorestr = String(Score);
  tft.fillScreen(ILI9341_BLUE);
  tft.setCursor(20, 100);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.println("SCORE : "+Scorestr);
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Score : ");
  Serial.println(Score);
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  Fin = "F;";
  Fin += Score;
  Fin += ";Reflex";
  BlueT.print(Fin);
}



void SimonCLI(int mode){ //jeuSimon
  uint8_t rotation=1;
  tft.setRotation(rotation);
  bool game = true;
  int j = 0;
  //int Tapp = 1000;
  int x1=100,y1=110,x2=120,y2=100;
  int x3=120,y3=30,x4=80,y4=60;
  String SuiteCouleur[40];
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(70, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("PARTIE EN COURS");
  tft.drawRect(x1-1, y1-1, x2+2, y2+2, ILI9341_WHITE);
  tft.drawRect(x3-1, y3-1, x4+2, y4+2, ILI9341_WHITE);

    viderBT();
    
  while (game) {
    j++;
    int i = 0;
    r1 = random(4);
    String Cr = couleur[r1];
    SuiteCouleur[j-1]=Cr;
    
    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.println(j);
    tft.drawRect(x1-1, y1-1, x2+2, y2+2, ILI9341_PURPLE);
    tft.drawRect(x3-1, y3-1, x4+2, y4+2, ILI9341_WHITE);
    for (i=0; i<j; i++){
      Serial.println(SuiteCouleur[i]);
      if (SuiteCouleur[i]=="jaune"){
        tft.fillRect(x1, y1, x2, y2, ILI9341_BLACK);
        tft.fillRect(x1, y1, x2, y2, ILI9341_YELLOW);
        }
      if (SuiteCouleur[i]=="rouge"){
        tft.fillRect(x1, y1, x2, y2, ILI9341_BLACK);
        tft.fillRect(x1, y1, x2, y2, ILI9341_RED);
        }
      if (SuiteCouleur[i]=="vert"){
        tft.fillRect(x1, y1, x2, y2, ILI9341_BLACK);
        tft.fillRect(x1, y1, x2, y2, ILI9341_GREEN);
        }
      if (SuiteCouleur[i]=="bleue"){
        tft.fillRect(x1, y1, x2, y2, ILI9341_BLACK);
        tft.fillRect(x1, y1, x2, y2, ILI9341_BLUE);
        }
      delay(100);
    }
    tft.fillRect(x1, y1, x2, y2, ILI9341_BLACK);
    viderBT();

    tft.drawRect(x1-1, y1-1, x2+2, y2+2, ILI9341_WHITE);
    tft.drawRect(x3-1, y3-1, x4+2, y4+2, ILI9341_PURPLE);
    for (i=0; i<j; i++){
      while (true) {
        delay(25);
        rep = "";
        rep = lireBT();
        Serial.println(rep);
      
        if (rep=="j") {
          if (SuiteCouleur[i]=="jaune") {
            //tft.drawCircle(100, 100, 50, ILI9341_YELLOW);
            //tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            tft.fillRect(x3, y3, x4, y4, ILI9341_YELLOW);
            tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            break;          
          }
          else {
            Serial.println("PERDU !!");
            game = false;
            break;
          }
        }
        
        if (rep=="r") {
          if (SuiteCouleur[i]=="rouge") {
            //tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            tft.fillRect(x3, y3, x4, y4, ILI9341_RED);
            tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }        
        }
        
        if (rep=="v") {
          if (SuiteCouleur[i]=="vert") {
            //tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            tft.fillRect(x3, y3, x4, y4, ILI9341_GREEN);
            tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }        
        }
        
        if (rep=="b") {
          if (SuiteCouleur[i]=="bleue") {
            //tft.fillTriangle(50, 50, 75, 100, 25, 100, ILI9341_BLUE);
            //tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            tft.fillRect(x3, y3, x4, y4, ILI9341_BLUE);
            tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }
        }
      }
    
      if (not game) {
        break;
      }
    }
    tft.fillRect(x3, y3, x4, y4, ILI9341_BLACK);
  }
  delay(500);
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Score : ");
  Serial.println(j-1);

  tft.fillScreen(ILI9341_BLUE);
  tft.setCursor(20, 100);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(5);
  tft.print("SCORE : ");
  tft.print(j-1);  
  
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  Fin = "F;";
  Fin += (j-1);
  Fin += ";Simon";
  BlueT.print(Fin);
}

void tracerCercle(int x, int y, String c){
  if(c== "rouge"){
    tft.fillCircle(x, y, 20, ILI9341_RED);
    }
   else if(c== "bleue"){
    tft.fillCircle(x, y, 20, ILI9341_BLUE);
    }
   else if(c== "jaune"){
    tft.fillCircle(x, y, 20, ILI9341_YELLOW);
    }
   else if(c== "vert"){
    tft.fillCircle(x, y, 20, ILI9341_GREEN);
    }
   else{
    tft.fillCircle(x, y, 20, ILI9341_WHITE);
    }
  }

 void tracerLigne(int y, String c[4]){
  tracerCercle(30, y, c[0]);
  tracerCercle(90, y, c[1]);
  tracerCercle(150, y, c[2]);
  tracerCercle(210, y, c[3]);
  }

  /*
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(70, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("PARTIE EN COURS");
  tft.drawRect(x1-1, y1-1, x2+2, y2+2, ILI9341_WHITE);
   */

void MastermindCLI(int mode){
  uint8_t rotation=1;
  tft.setRotation(rotation);  
  tft.fillScreen(ILI9341_BLACK);
  int nbR = 0;
  int nbV = 0;
  int nbB = 0;
  int nbJ = 0;
  int RnbR = 0;
  int RnbV = 0;
  int RnbB = 0;
  int RnbJ = 0;
  int Bcouleur = 0;
  int Bordre = 0;
  int Ess = 1;
  int i = 0;
  int b1 = 0;
  int b2 = 0;
  int b3 = 0;
  int b4 = 0;
  int m1 = 0;
  int m2 = 0;
  int m3 = 0;
  int m4 = 0;
  
  String SCouleur[4];
  String RepCouleur[4];
  String RepL1[4] = {"x","x","x","x"};
  String RepL2[4] = RepL1[4];
  String RepL3[4] = RepL1[4];

  if (mode==1) {
    for(i=0; i<4; i++) {
      int r1 = random(4);
      SCouleur[i] = couleur[r1];
      if (r1 == 0) {
        nbR++;
      }
      else if (r1 == 1) {
        nbV++;
      }
      else if (r1 == 2) {
        nbB++;
      }
      else {
        nbJ++;
      }
    }
  }
  else {
    Serial.println("Joueur 1 choisissez la suite a deviné");
    viderBT();
    while(i<4){
      delay(25);
      rep = "";
      rep = lireBT();
      
      if (rep == "r") {
        SCouleur[i] = "rouge";
        Serial.print("rouge,");
        nbR++;
        i++;
      }
      else if (rep == "v") {
        SCouleur[i] = "vert";
        Serial.print("vert,");
        nbV++;
        i++;
      }
      else if (rep == "b") {
        SCouleur[i] = "bleue";
        Serial.print("bleue,");
        nbB++;
        i++;
      }
      else if (rep == "j") {
        SCouleur[i] = "jaune";
        Serial.print("jaune,");
        nbJ++;
        i++;
      }
    }
    Serial.println("");
    Serial.println("Joueur 2 a vous de jouer");
  }

  Serial.println("~~~~~~~~~~~~~");
  Serial.println("C'est parti !!");
  while (true) {
    Serial.print("Essaie(s) n°");
    Serial.print(Ess);
    Serial.println(" :");
    i = 0;
    Bcouleur = 0;
    Bordre = 0;
    RnbR = 0;
    RnbV = 0;
    RnbB = 0;
    RnbJ = 0;
    viderBT();
    
    while(i<4){
      delay(25);
      rep = "";
      rep = lireBT();
      
      if (rep == "r") {
        RepCouleur[i] = "rouge";
        Serial.print("rouge,");
        RnbR++;
        i++;
      }
      else if (rep == "v") {
        RepCouleur[i] = "vert";
        Serial.print("vert,");
        RnbV++;
        i++;
      }
      else if (rep == "b") {
        RepCouleur[i] = "bleue";
        Serial.print("bleue,");
        RnbB++;
        i++;
      }
      else if (rep == "j") {
        RepCouleur[i] = "jaune";
        Serial.print("jaune,");
        RnbJ++;
        i++;
      }
    }
    
    Bcouleur = min(RnbR,nbR);
    Bcouleur += min(RnbV,nbV);
    Bcouleur += min(RnbB,nbB);
    Bcouleur += min(RnbJ,nbJ);
    for(i=0;i<4;i++) {
      if(SCouleur[i]==RepCouleur[i]){
        Bcouleur--;
        Bordre++;
      }
    }
    if (Bordre==4) {
      break;
    }
    else {
      Ess++;
    }
    Serial.println("");
    Serial.print("Couleur Mal Placé : ");
    Serial.println(Bcouleur);
    Serial.print("Couleur Bien Placé : ");
    Serial.println(Bordre);
    Serial.println("~~~~~~~~~~~~~");
    
    tracerLigne(30, RepL1);
    tracerLigne(90, RepL2);
    tracerLigne(150, RepL3);
    tracerLigne(210, RepCouleur);

    for(int k=0; k<4; k++){
     RepL1[k] = RepL2[k];
     RepL2[k] = RepL3[k];
     RepL3[k] = RepCouleur[k];
      }

      //infos bons placements

      m4 = Bcouleur;
      b4 = Bordre;
      
    tft.fillRect(250, 0, 70, 240, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    
    tft.setCursor(260, 30);
    tft.print(b1);
    tft.print("  ");
    tft.print(m1);
    tft.setCursor(260, 90);
    tft.print(b2);
    tft.print("  ");
    tft.print(m2);
    tft.setCursor(260, 150);
    tft.print(b3);
    tft.print("  ");
    tft.print(m3);
    tft.setCursor(260, 210);
    tft.print(b4);
    tft.print("  ");
    tft.print(m4);

     m1 = m2;
     m2 = m3;
     m3 = m4;
     b1 = b2;
     b2 = b3;
     b3 = b4;
     

  }
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("Bravo !!!");
  Serial.print("Essai(s) : ");
  Serial.println(Ess);
  Serial.println("~~~~~Fini~~~~~");
  tft.fillScreen(ILI9341_BLUE);
        tft.setCursor(40, 100);
        tft.setTextColor(ILI9341_WHITE);  
        tft.setTextSize(8);
        tft.print("BRAVO :");
        tft.println(Ess);
        
  viderBT();
  Fin = "F;";
  Fin += Ess;
  Fin += ";Mastermind";
  BlueT.print(Fin);
}



void BlackJackCLI(int mode){
  int dejaT = 0;
  int JeuB[3] = {0,0,0};
  int JeuJ1[3] = {0,0,0};
  int JeuJ2[3] = {0,0,0};
  int EtP[3] = {0,0,0};

  Serial.println("~~~~~~~~~~~~~");
  Serial.println("C'est parti !!");

  //Tirage premiere carte Banque
  Serial.print("La Carte de la Banque: ");
  tirageC(JeuB[0],JeuB[1]);
  JeuB[0] = Rec[0];
  JeuB[1] = Rec[1];
  JeuB[2]++;
  
  //Tirage premiere carte Joueur
  Serial.print("Carte du ");
  if (mode==2){Serial.print("Premier ");}
  Serial.print("Joueur: ");
  tirageC(JeuJ1[0],JeuJ1[1]);
  JeuJ1[0] = Rec[0];
  JeuJ1[1] = Rec[1];
  JeuJ1[2]++;

  //Tirage deuxieme carte Joueur
  if (mode==2) {
    Serial.print("Carte du Deuxième Joueur: ");
    tirageC(JeuJ2[0],JeuJ2[1]);
    JeuJ2[0] = Rec[0];
    JeuJ2[1] = Rec[1];
    JeuJ2[2]++;
  }

  //Tour du Joueur 1
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Au tour du ");
  if (mode==2){Serial.print("Premier ");}
  Serial.println("Joueur");
  Serial.print("Total Joueur Actuel: ");
  Serial.println(JeuJ1[0]);
  viderBT();
  while (true) {
    rep = "";
    rep = lireBT();
    
    if (rep=="C"){
      Serial.print("Nouvelle Carte du Joueur: ");
      tirageC(JeuJ1[0],JeuJ1[1]);
      JeuJ1[0] = Rec[0];
      JeuJ1[1] = Rec[1];
      JeuJ1[2]++;
      Serial.print("Total Joueur Actuel: ");
      Serial.println(JeuJ1[0]);
      if (JeuJ1[0]>21){
        dejaT+=1;
        EtP[1] = 1;
        delay(10);
        break;
      }
    }
    else if (rep=="R") {
        break;
    }
    delay(25);
  }

  //Tour du Joueur 2
  if (mode==2) {
    Serial.println("~~~~~~~~~~~~~");
    Serial.println("Au tour du Deuxième Joueur");
    Serial.print("Total Joueur Actuel: ");
    Serial.println(JeuJ2[0]);
    viderBT();
    while (true) {
      rep = "";
      rep = lireBT();
      
      if (rep=="C"){
        Serial.print("Nouvelle Carte du Joueur: ");
        tirageC(JeuJ2[0],JeuJ2[1]);
        JeuJ2[0] = Rec[0];
        JeuJ2[1] = Rec[1];
        JeuJ2[2]++;
        Serial.print("Total Joueur Actuel: ");
        Serial.println(JeuJ2[0]);
        if (JeuJ2[0]>21){
            dejaT+=2;
            EtP[2]=1;
            delay(10);
            break;
          }
        }
        else if (rep=="R") {
          break;
        }
        delay(25);
    }
  }
  
  //Tour de la Banque
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("A la banque de jouer");
  Serial.print("Total Banque Actuel: ");
  Serial.println(JeuB[0]);
  while ( (JeuB[0]<17) and ( ((mode==1)and(EtP[1]==0))or((mode==2)and(EtP[1]==0)and(EtP[2]==0)) ) ) {
    Serial.print("Nouvelle Carte Banque: ");
    tirageC(JeuB[0],JeuB[1]);
    JeuB[0] = Rec[0];
    JeuB[1] = Rec[1];
    JeuB[2]++;
    Serial.print("Total Banque Actuel: ");
    Serial.println(JeuB[0]);
    if (JeuB[0]>21) {
        EtP[0] = 1;
    }
  }

    //Calcul Gagnant
    Serial.println("~~~~~~~~~~~~~~");
    
    if (EtP[0]==1) {
      if (EtP[1]==0) {
        Serial.println("Le Premier Joueur remporte la victoire");
      }
      if ((EtP[2]==0)and(mode==2)) {
        Serial.println("Le Deuxième Joueur remporte la victoire");
      }
    }
    else {
      if (EtP[1]==0) {
        if (JeuJ1[0] > JeuB[0]) {
          Serial.println("Le Premier Joueur remporte la victoire");
        }
        else if (JeuB[0]==JeuJ1[0]) {
          if (JeuJ1[2] >= JeuB[2]) {
            Serial.println("Le Premier Joueur remporte la victoire");
          }
          else {
            EtP[1]=1;
          }
        }
        else {
          EtP[1]=1;
        }
      }
      if ((EtP[2]==0)and(mode==2)) {
        if (JeuJ2[0] > JeuB[0]) {
          Serial.println("Le Deuxième Joueur remporte la victoire");
        }
        else if (JeuB[0]==JeuJ1[2]) {
          if (JeuJ2[2] >= JeuB[2]) {
            Serial.println("Le Deuxième Joueur remporte la victoire");
          }
          else {
            EtP[2]=1;
          }
        }
        else {
          EtP[2]=1;
        }
      }
    }
    if ( ((mode==1)and(EtP[1]==1)) or ((EtP[1]==1)and(EtP[2]==1)and(mode==2)) ) {
      if (mode==1) {Serial.println("Le Joueur a perdu ...");}
      if (mode==2) {Serial.println("Les Joueurs ont perdu ...");}
    }
    
    Serial.println("~~~~~Fini~~~~~");
    viderBT();
    BlueT.print("F");
}

int calBJ(int a) {
  int res = 0;
    if (a==0) {
    res += 11;
  }
  else if (a<9) {
    res += a+1;
  }
  else {
    res += 10;
  }
  return res;
}

void tirageC(int a, int b) {
  int r1 = random(4);
  int r2 = random(13);
  if (r2==0) {
    b++;
  }
  a += calBJ(r2);
  if (a>21){
    if (b>0) {
      a -= 10;
      b--;
    }
  }
  Serial.print(Valeur[r2]);
  Serial.print(" de ");
  Serial.println(Couleur[r1]);
  Rec[0]=a;
  Rec[1]=b;
}

void DessinerDe(int v, int x, int y){
  if(x==64){
    tft.fillRect(x+1, y+1, 63, 62, ILI9341_BLACK);
    }
   else if(x==256){
    tft.fillRect(x, y+1, 63, 62, ILI9341_BLACK);
    }
   else if(x==0){
    tft.fillRect(x+1, y+1, 63, 62, ILI9341_BLACK);
    }
  else{
    tft.fillRect(x+1, y+1, 62, 62, ILI9341_BLACK);
    }
  
  if(v==1){
    tft.fillCircle(x+32, y+32, 4, ILI9341_WHITE);  
    }
  if(v==2){
    tft.fillCircle(x+16, y+16, 4, ILI9341_WHITE);
    tft.fillCircle(x+48, y+48, 4, ILI9341_WHITE);
    }
  if(v==3){
    tft.fillCircle(x+16, y+16, 4, ILI9341_WHITE);
    tft.fillCircle(x+32, y+32, 4, ILI9341_WHITE);  
    tft.fillCircle(x+48, y+48, 4, ILI9341_WHITE);
    }
  if(v==4){
      tft.fillCircle(x+16, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+16, y+48, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+48, 4, ILI9341_WHITE);
    }
  if(v==5){
      tft.fillCircle(x+16, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+32, y+32, 4, ILI9341_WHITE);
      tft.fillCircle(x+16, y+48, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+48, 4, ILI9341_WHITE);
    }
  if(v==6){
      tft.fillCircle(x+16, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+16, 4, ILI9341_WHITE);
      tft.fillCircle(x+16, y+32, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+32, 4, ILI9341_WHITE);
      tft.fillCircle(x+16, y+48, 4, ILI9341_WHITE);
      tft.fillCircle(x+48, y+48, 4, ILI9341_WHITE);
    }  
  }

void Dessiner5De(int j[]){
  DessinerDe(j[0],0,0);
  DessinerDe(j[1],64,0);
  DessinerDe(j[2],128,0);
  DessinerDe(j[3],192,0);
  DessinerDe(j[4],256,0);
  }

void Select(int n){
  switch (n) {
  case 1:
    tft.drawRect(1, 1, 63, 62, ILI9341_GREEN);
    tft.drawRect(2, 2, 61, 60, ILI9341_GREEN);
    break;
  case 2:
    tft.drawRect(65, 1, 63, 62, ILI9341_GREEN);
    tft.drawRect(66, 2, 61, 60, ILI9341_GREEN);
    break;
  case 3:
    tft.drawRect(129, 1, 62, 62, ILI9341_GREEN);
    tft.drawRect(130, 2, 60, 60, ILI9341_GREEN);
    break;
  case 4:
    tft.drawRect(192, 1, 63, 62, ILI9341_GREEN);
    tft.drawRect(193, 2, 61, 60, ILI9341_GREEN);
    break;
  case 5:
    tft.drawRect(256, 1, 63, 62, ILI9341_GREEN);
    tft.drawRect(257, 2, 61, 60, ILI9341_GREEN);
    break;
  }
}

void Unselect(int n){
  switch (n) {
  case 1:
    tft.drawRect(1, 1, 63, 62, ILI9341_BLACK);
    tft.drawRect(2, 2, 61, 60, ILI9341_BLACK);
    break;
  case 2:
    tft.drawRect(65, 1, 63, 62, ILI9341_BLACK);
    tft.drawRect(66, 2, 61, 60, ILI9341_BLACK);
    break;
  case 3:
    tft.drawRect(129, 1, 62, 62, ILI9341_BLACK);
    tft.drawRect(130, 2, 60, 60, ILI9341_BLACK);
    break;
  case 4:
    tft.drawRect(192, 1, 63, 62, ILI9341_BLACK);
    tft.drawRect(193, 2, 61, 60, ILI9341_BLACK);
    break;
  case 5:
    tft.drawRect(256, 1, 63, 62, ILI9341_BLACK);
    tft.drawRect(257, 2, 61, 60, ILI9341_BLACK);
    break;
  }
}

void YamsCLI(int mode) {
  int JeuJ1[5];
  int JeuJ2[5];
  int ScoreJ1 = 0;
  int ScoreJ2 = 0;
  int Change[5]={9,9,9,9,9};
  int aTire=1;
  int count;
  int paire = 0;
  int brel = 0;
  int carre = 0;
  bool yams = 0;
  int Tsuite[2];
  int Oscour = 0;
  int rnd = 0;

  uint8_t rotation=1;
  tft.setRotation(rotation);
  
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0, 0, 320, 64, ILI9341_WHITE);
  tft.drawRect(64, 0, 192, 64, ILI9341_WHITE);
  tft.drawRect(128, 0, 64, 64, ILI9341_WHITE);

  while (rnd<3) {
    viderBT();
    aTire=1;
    count;
    paire = 0;
    brel = 0;
    carre = 0;
    yams = 0;
    Oscour = 0;
    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.print(rnd+1);
    Serial.println("/3");
    rnd++;

    
    //Premier Lancer J1
    tft.fillRect(0, 65, 320, 60, ILI9341_BLACK);
    tft.setCursor(10, 70);
    tft.setTextColor(ILI9341_BLUE);  
    tft.setTextSize(2);
    tft.println("Premier tirage Joueur 1 :");
    Serial.println("~~~~~~~~~~~~~");
    Serial.println("Premier lancé Joueur 1 :");
    Lancer(5);
    for (int i =0; i<5;i++) {
      JeuJ1[i]=Rec[i];
      Serial.print("Dés n°");
      Serial.print(i+1);
      Serial.print(" -> ");
      Serial.println(JeuJ1[i]);
    }
    
    Dessiner5De(JeuJ1);
    
    viderBT();
    
    while (aTire<3) {
      for (int i =0; i<5;i++){
        Change[i]=9;
      }
      count=0;
      Serial.println("~~~~~~~~~~~~~");
      Serial.println("Dé(s) a relancer ?");
      while (true) {
        delay(25);
        rep = lireBT();
    
        if ((rep == "A")or(rep == "B")or(rep == "C")or(rep == "D")or(rep == "E")) {
          if (rep == "A") {
            if (Change[0] == 9 ) {
              Change[0]=0;
            }
            else {
              Change[0]=9;
            }
          }
          
          else if (rep == "B") {
            if (Change[1] == 9 ) {
              Change[1]=1;
            }
            else {
              Change[1]=9;
            }
          }
          
          else if (rep == "C") {
            if (Change[2] == 9 ) {
              Change[2]=2;
            }
            else {
              Change[2]=9;
            }
          }
          else if (rep == "D") {
            if (Change[3] == 9  ) {
              Change[3]=3;
            }
            else {
              Change[3]=9;
            }
          }
          
          else if (rep == "E") {
            if (Change[4] == 9 ) {
              Change[4]=4;
            }
            else {
              Change[4]=9;
            }
          }
          Serial.print("Dé(s) a relancer :");
          
          for (int i =0; i<5;i++){
            if (Change[i] != 9) {
            Serial.print("n°");
            Serial.print(Change[i]+1);
            Select(i+1);
            Serial.print(" ");
            }
            else{
              Unselect(i+1);
              }
          }
          Serial.println("");
        }
    
        else if (rep == "R") {
          for (int i=0;i<5;i++) {
            if (Change[i]!=9){count++;}
          }
          if (count == 0) {
          Serial.println("Aucun dé a relancer");
          }
          else {
            aTire++;
            break;
          }
        }
        else if (rep == "G") {
          aTire = 4;
          break;
        }
      }
      
      if (aTire<4) {
        Lancer(count);
        Oscour = 0;
        Serial.println("Nouveau lancé :");
        tft.fillRect(0, 65, 320, 60, ILI9341_BLACK);
        tft.setCursor(50, 70);
        tft.setTextColor(ILI9341_BLUE);  
        tft.setTextSize(2);
        tft.println("Nouveau tirage");
        for (int i = 0; i<5;i++){
          if (Change[i] == i) {
            JeuJ1[Change[i]]=Rec[Oscour];
            Oscour++;
          }
        }
        for (int i =0; i<5;i++) {
          Serial.print("Dés n°");
          Serial.print(i+1);
          Serial.print(" -> ");
          Serial.println(JeuJ1[i]);
        }
        Dessiner5De(JeuJ1);
        delay(1000);
      }
    }
  
    //Calcul Score J1
    paire = 0;
    brel = 0;
    carre = 0;
    yams = false;
    for (int i =0; i<5;i++){
      if (i<2) {Tsuite[i]=0;}
      Change[i]=9;
    }
    
    for (int i=0;i<5;i++){
      count=0;
      Tsuite[0]=0;
      
      if(Change[i]!=i){
        for(int j=0;j<5;j++){
          if((i!=j)and(JeuJ1[i]==JeuJ1[j])) {
            count++;
            Change[i]=i;
            Change[j]=j;
          }
        }
        if (count == 1) {paire=JeuJ1[i];}
        else if (count == 2) {brel=JeuJ1[i];}
        else if (count == 3) {carre=JeuJ1[i];}
        else if (count == 4) {yams=true;}
      }
      
      if (JeuJ1[i] < 4) {
        testSuite :
        for(int j=0;j<5;j++){
          if (JeuJ1[i]+Tsuite[0]+1==JeuJ1[j]) {
            Tsuite[0]++;
            goto testSuite;
          }
          else if (Tsuite[1]<Tsuite[0]) {
            Tsuite[1]=Tsuite[0];
          }
        }
      }
    }
    Serial.println("~~~~~~~~~~~~~");
    if (yams) {
      ScoreJ1+=50;
      Serial.println("Le joueur 1 a un YAMS !!");
      Serial.println("Score +50");
    }
    else if (carre != 0) {
      ScoreJ1+=4*carre;
      Serial.println("~~~~~~~~~~~~~");
      Serial.print("Le joueur 1 a un Carré de ");
      Serial.println(carre);
      Serial.print("Score +");
      Serial.println(4*carre);
    }
    else if ((brel !=0)and(paire!=0)) {
      ScoreJ1+=25;
      Serial.println("Le joueur 1 a un Full");
      Serial.println("Score +25");
    }
    else if (brel !=0) {
      ScoreJ1+=3*brel;
      Serial.print("Le joueur 1 a un Brelan de ");
      Serial.println(brel);
      Serial.print("Score +");
      Serial.println(3*brel);
    }
    else if (Tsuite[1]==3) {
      ScoreJ1+=30;
      Serial.println("Le joueur 1 a une Petite Suite");
      Serial.println("Score +30");
    }
    else if (Tsuite[1]==4) {
      ScoreJ1+=40;
      Serial.println("Le joueur 1 a une Grande Suite");
      Serial.println("Score +40");
    }
    else {
      Serial.println("Le joueur 1 n'a rien . . .");
      Serial.println("Score +0");
    }
    tft.fillRect(5, 210, 170, 20, ILI9341_BLACK);
      tft.setCursor(5, 210);
      tft.setTextColor(ILI9341_BLUE);  
      tft.setTextSize(2);
      tft.print("Score J1:");
      tft.print(ScoreJ1);

    viderBT();
    aTire=1;
    count;
    paire = 0;
    brel = 0;
    carre = 0;
    yams = 0;
    Oscour = 0;
    
  //Premier Lancer J2
    tft.fillRect(0, 65, 320, 60, ILI9341_BLACK);
    tft.setCursor(10, 70);
    tft.setTextColor(ILI9341_RED);  
    tft.setTextSize(2);
    tft.println("Premier tirage Joueur 2 :");
    Serial.println("~~~~~~~~~~~~~");
    Serial.println("Premier lancé Joueur 2 :");
    Lancer(5);
    for (int i =0; i<5;i++) {
      JeuJ2[i]=Rec[i];
      Serial.print("Dés n°");
      Serial.print(i+1);
      Serial.print(" -> ");
      Serial.println(JeuJ2[i]);
    }

    Dessiner5De(JeuJ2);
    viderBT();
    
    while (aTire<3) {
      for (int i =0; i<5;i++){
        Change[i]=9;
      }
      count=0;
      Serial.println("~~~~~~~~~~~~~");
      Serial.println("Dé(s) a relancer ?");
      while (true) {
        delay(25);
        rep = lireBT();
    
        if ((rep == "A")or(rep == "B")or(rep == "C")or(rep == "D")or(rep == "E")) {
          if (rep == "A") {
            if (Change[0] == 9 ) {
              Change[0]=0;
            }
            else {
              Change[0]=9;
            }
          }
          
          else if (rep == "B") {
            if (Change[1] == 9 ) {
              Change[1]=1;
            }
            else {
              Change[1]=9;
            }
          }
          
          else if (rep == "C") {
            if (Change[2] == 9 ) {
              Change[2]=2;
            }
            else {
              Change[2]=9;
            }
          }
          else if (rep == "D") {
            if (Change[3] == 9  ) {
              Change[3]=3;
            }
            else {
              Change[3]=9;
            }
          }
          
          else if (rep == "E") {
            if (Change[4] == 9 ) {
              Change[4]=4;
            }
            else {
              Change[4]=9;
            }
          }
          Serial.print("Dé(s) a relancer :");
          
          for (int i =0; i<5;i++){
            if (Change[i] != 9) {
            Serial.print("n°");
            Serial.print(Change[i]+1);
            Serial.print(" ");
            Select(i+1);
            }
            else{
              Unselect(i+1);
              }
          }
          Serial.println("");
        }
    
        else if (rep == "R") {
          for (int i=0;i<5;i++) {
            if (Change[i]!=9){count++;}
          }
          if (count == 0) {
          Serial.println("Aucun dé a relancer");
          }
          else {
            aTire++;
            break;
          }
        }
        else if (rep == "G") {
          aTire = 4;
          break;
        }
      }
      
      if (aTire<4) {
        Lancer(count);
        Oscour = 0;
        tft.fillRect(0, 65, 320, 60, ILI9341_BLACK);
        tft.setCursor(50, 70);
        tft.setTextColor(ILI9341_RED);  
        tft.setTextSize(2);
        tft.println("Nouveau tirage");
        Serial.println("Nouveau lancé :");
        for (int i = 0; i<5;i++){
          if (Change[i] == i) {
            JeuJ2[Change[i]]=Rec[Oscour];
            Oscour++;
          }
        }
        for (int i =0; i<5;i++) {
          Serial.print("Dés n°");
          Serial.print(i+1);
          Serial.print(" -> ");
          Serial.println(JeuJ2[i]);
        }
        Dessiner5De(JeuJ2);
        delay(1000);
      }
    }
  
    //Calcul Score J2
    paire = 0;
    brel = 0;
    carre = 0;
    yams = false;
    for (int i =0; i<5;i++){
      if (i<2) {Tsuite[i]=0;}
      Change[i]=9;
    }
    
    for (int i=0;i<5;i++){
      count=0;
      Tsuite[0]=0;
      
      if(Change[i]!=i){
        for(int j=0;j<5;j++){
          if((i!=j)and(JeuJ2[i]==JeuJ2[j])) {
            count++;
            Change[i]=i;
            Change[j]=j;
          }
        }
        if (count == 1) {paire=JeuJ2[i];}
        else if (count == 2) {brel=JeuJ2[i];}
        else if (count == 3) {carre=JeuJ2[i];}
        else if (count == 4) {yams=true;}
      }
      
      if (JeuJ2[i] < 4) {
        testSuite2 :
        for(int j=0;j<5;j++){
          if (JeuJ2[i]+Tsuite[0]+1==JeuJ2[j]) {
            Tsuite[0]++;
            goto testSuite2;
          }
          else if (Tsuite[1]<Tsuite[0]) {
            Tsuite[1]=Tsuite[0];
          }
        }
      }
    }
    
    Serial.println("~~~~~~~~~~~~~");
    if (yams) {
      ScoreJ2+=50;
      Serial.println("Le joueur 2 a un YAMS !!");
      Serial.println("Score +50");
      }
    else if (carre != 0) {
      ScoreJ2+=4*carre;
      Serial.print("Le joueur 2 a un Carré de ");
      Serial.println(carre);
      Serial.print("Score +");
      Serial.println(4*carre);
    }
    else if ((brel !=0)and(paire!=0)) {
      ScoreJ2+=25;
      Serial.println("Le joueur 2 a un Full");
      Serial.println("Score +25");
    }
    else if (brel !=0) {
      ScoreJ2+=3*brel;
      Serial.print("Le joueur 2 a un Brelan de ");
      Serial.println(brel);
      Serial.print("Score +");
      Serial.println(3*brel);
    }
    else if (Tsuite[1]==3) {
      ScoreJ2+=30;
      Serial.println("Le joueur 2 a une Petite Suite");
      Serial.println("Score +30");
    }
    else if (Tsuite[1]==4) {
      ScoreJ2+=40;
      Serial.println("Le joueur 2 a une Grande Suite");
      Serial.println("Score +40");
    }
    else {
      Serial.println("Le joueur 2 n'a rien . . .");
      Serial.println("Score +0");
    }
    tft.fillRect(175, 210, 145, 20, ILI9341_BLACK);
      tft.setCursor(175, 210);
      tft.setTextColor(ILI9341_RED);  
      tft.setTextSize(2);
      tft.print("Score J2:");
      tft.print(ScoreJ2);
  }
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Scores finaux :     J1 : ");
  Serial.print(ScoreJ1);
  Serial.print("        J2 : ");
  Serial.println(ScoreJ2);
  Fin = "F;";
  if (ScoreJ1>ScoreJ2) {
      Fin += ScoreJ1;
    Serial.println("Le joueur 1 remporte la partie");
    tft.fillScreen(ILI9341_BLUE);
    tft.setCursor(40, 100);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(5);
      tft.println("J1 GAGNE");
  }
  else if(ScoreJ1<ScoreJ2){
      Fin += ScoreJ2;
    Serial.println("Le joueur 2 remporte la partie");
    tft.fillScreen(ILI9341_RED);
    tft.setCursor(40, 100);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(5);
      tft.println("J2 GAGNE");
  }
  else{
      Fin += ScoreJ2;
    Serial.println("Egalité !");
    tft.fillScreen(ILI9341_GREEN);
    tft.setCursor(40, 100);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(6);
      tft.println("EGALITE");
  }
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  Fin += ";Yams";
  BlueT.print(Fin);
}

void Lancer(int a) {
  for (int i = 0; i<a;i++) {
    r1 = random(6);
    Rec[i]=r1+1;
  }
}

void PenduCLI(int mode) {
  uint8_t rotation=1;
  tft.setRotation(rotation);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(70, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println("PARTIE EN COURS");
  String test;
  String mot;
  String etat;
  String retour;
  int err = 0;

  if (mode==1) {
    viderBT();
    BlueT.print("P");
    delay(1000);
    mot = lireBT();
  }
  else {
    viderBT();
    BlueT.print("P");
    delay(1000);
    mot=lireBT();
    while (true) {
      if (mot.length()>0) {
        mot="";
        viderBT();
        break;
      }
    }
    while (true) {
      mot=lireBT();
      if (mot.length()>0) {
        Serial.println(mot);
        break;
      }
    }
  }
  for (int i = 0 ; i<mot.length();i++) {
    if (mot[i] == ' ') {
      etat+=" ";
    }
    else if (mot[i] == '\'') {
      etat+="\'";
    }
    else if (mot[i] == '-') {
      etat+="-";
    }
    else {
      etat+=".";
    }
  }
  Serial.println(mot);
  
  for (int r = 1; r>0 ; r++) {
    if (penduWin(etat)) {
        tft.fillScreen(ILI9341_BLUE);
        tft.setCursor(40, 100);
        tft.setTextColor(ILI9341_WHITE);  
        tft.setTextSize(8);
        tft.println("BRAVO");
        Serial.println("Le joueur remporte la victoire !");
        break;
    }
    if (err==10) {
      tft.fillScreen(ILI9341_RED);
      tft.setCursor(40, 100);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(8);
      tft.println("PERDU");
      Serial.println("Le joueur perd la partie ..."); 
      break;
    }
    else {
      tft.fillRect(20, 40, 300, 20, ILI9341_BLACK);
      String errstr = String(10-err);
      tft.setCursor(20, 40);
      tft.setTextColor(ILI9341_WHITE);  
      tft.setTextSize(2);
      tft.print("Plus que ");
      tft.print(errstr);
      tft.print(" erreur(s)");
      Serial.print("Plus que ");
      Serial.print(10-err);
      Serial.println(" erreur(s) possible(s)");
    }
    viderBT();
    Serial.println("~~~~~~~~~~~~~~");
    Serial.print("round : ");
    Serial.println(r);
    penduTest(test);
    Serial.println(etat);
    tft.fillRect(70, 100, 250, 50, ILI9341_BLACK);
    tft.setCursor(70, 100);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println(etat);
    Serial.print("Lettre ? ");
    while (true) {
      rep = lireBT();
      if (isLowerCase(rep[0])) {
        Serial.print(rep[0]);
        test+=rep[0];
        retour=penduMAJ(test,mot,etat);
        if (retour.equals(etat)) {
          Serial.println(" ✗");
          err++;
        }
        else {
          Serial.println(" ✓");
        }
        etat=retour;
        break;
      }
      delay(50);
    }
  }
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  BlueT.print("F");
}

String penduMAJ(String test, String mot, String etat) {
  String retour=etat;
  for (int i = 0;i<test.length();i++) {
    for (int j = 0;j<mot.length();j++) {
      if(mot[j]==test[i]) {
        retour[j]=test[i];
      }
    }
  }
  return retour;
}

void penduTest(String test) {
  Serial.print("Lettre déja testé : ");
  tft.setCursor(10, 200);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  for (int i = 0; i<test.length(); i++){
    Serial.print(test[i]);
    Serial.print(",");
    tft.print(test[i]);
    tft.print(",");
  }
  Serial.println("");
}

bool penduWin(String etat) {
  for (int i = 0; i < etat.length() ; i++) {
    if (etat[i]=='.') {
      return false;
    }
  }
  return true;
}

void TicTacToe(int mode) {
  resetBoardTTT();
  int Tour = 1;
  int Etat = 0;
  int Win = 0;
  uint8_t rotation=1;
  tft.setRotation(rotation);
  
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(40, 0, 240, 240, ILI9341_WHITE);
  tft.drawRect(120, 0, 80, 240, ILI9341_WHITE);
  tft.drawRect(40, 80, 240, 80, ILI9341_WHITE);

  while (true) {
    Serial.println("~~~~~~~~~~~~~~");
    Serial.print("Au tour du Joueur ");
    Serial.println(Tour);
    
    affBoardTTT();
    viderBT();

    if(Tour==1){
      tft.fillRect(280, 0, 40, 240, ILI9341_BLACK);
      tft.fillRect(0, 0, 40, 240, ILI9341_BLUE);
      }
    else {
      tft.fillRect(0, 0, 40, 240, ILI9341_BLACK);
      tft.fillRect(280, 0, 40, 240, ILI9341_RED);
      }
    
    while(Etat==0){
      rep = lireBT();
      if(rep=="A1") {
        if (L1[0]==0) {
          L1[0]=Tour;
          if(Tour==1){
            tft.drawCircle(80, 200, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(80, 200, 30, ILI9341_RED);
              }          
          }
        Etat++;
      }
      if(rep=="A2") {
        if (L2[0]==0) {
          L2[0]=Tour;
          if(Tour==1){
            tft.drawCircle(80, 120, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(80, 120, 30, ILI9341_RED);
              }  
          }
        Etat++;
      }
      if(rep=="A3") {
        if (L3[0]==0) {
          L3[0]=Tour;
          if(Tour==1){
            tft.drawCircle(80, 40, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(80, 40, 30, ILI9341_RED);
              }  
          }
        Etat++;
      }
      if(rep=="B1") {
        if (L1[1]==0) {
          L1[1]=Tour;
          if(Tour==1){
            tft.drawCircle(160, 200, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(160, 200, 30, ILI9341_RED);
              }
          }
        Etat++;
      }
      if(rep=="B2") {
        if (L2[1]==0) {
          L2[1]=Tour;          
          if(Tour==1){
            tft.drawCircle(160, 120, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(160, 120, 30, ILI9341_RED);
              }}
        Etat++;
      }
      if(rep=="B3") {
        if (L3[1]==0) {
          L3[1]=Tour;
          if(Tour==1){
            tft.drawCircle(160, 40, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(160, 40, 30, ILI9341_RED);
              }
          }
        Etat++;
      }
      if(rep=="C1") {
        if (L1[2]==0) {
          L1[2]=Tour;
          if(Tour==1){
            tft.drawCircle(240, 200, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(240, 200, 30, ILI9341_RED);
              }
          }
        Etat++;
      }
      if(rep=="C2") {
        if (L2[2]==0) {
          L2[2]=Tour;
          if(Tour==1){
            tft.drawCircle(240, 120, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(240, 120, 30, ILI9341_RED);
              }
          }
        Etat++;
      }
      if(rep=="C3") {
        if (L3[2]==0) {
          L3[2]=Tour;          
          if(Tour==1){
            tft.drawCircle(240, 40, 30, ILI9341_BLUE);
            }
            else{
              tft.drawCircle(240, 40, 30, ILI9341_RED);
              }
            }
        Etat++;
      }
      delay(25);
    }
    Etat--;
    Win = checkWinTTT();
    if (Win>0) {
      break;
    }
    if (Tour==1) {Tour++;}
    else {Tour--;}
  }
  Serial.println("~~~~~~~~~~~~~~");
  affBoardTTT();
  Serial.println("~~~~~~~~~~~~~~");
  if (Win==1) {
    tft.fillScreen(ILI9341_BLACK); 
    tft.setTextSize(4);
    if(Tour==1){
      tft.setCursor(40, 100);
      tft.setTextColor(ILI9341_BLUE);
      tft.println("BLEU GAGNE"); 
      }
    else{
      tft.setCursor(30, 100);
      tft.setTextColor(ILI9341_RED);
      tft.println("ROUGE GAGNE"); 
      }
    Serial.print("Le Joueur ");
    Serial.print(Tour);
    Serial.println(" a gagné !!!");
  }
  else {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(40, 100);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(6);
    tft.println("EGALITE");
    Serial.println("Egalité");
  }
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  BlueT.print("F");
}

void affBoardTTT() {
  for(int i = 0;i<3;i++) {
    Serial.print(L3[i]);
    if (i<2) {
      Serial.print("|");
    }
  }
  Serial.println("");
  Serial.println("-----");
  for(int i = 0;i<3;i++) {
    Serial.print(L2[i]);
    if (i<2) {
      Serial.print("|");
    }
  }
  Serial.println("");
  Serial.println("-----");
  for(int i = 0;i<3;i++) {
    Serial.print(L1[i]);
    if (i<2) {
      Serial.print("|");
    }
  }
  Serial.println("");
}

void resetBoardTTT() {
  for(int i = 0;i<3;i++) {
    L1[i]=0;
  }
  for(int i = 0;i<3;i++) {
    L2[i]=0;  
  }
  for(int i = 0;i<3;i++) {
    L3[i]=0;
  }
}

int checkWinTTT(){
  if((L1[0]>0)and(L1[1]>0)and(L1[2]>0)and(L2[0]>0)and(L2[1]>0)and(L2[2]>0)and(L3[0]>0)and(L3[1]>0)and(L3[2]>0)) {return 2;}
  if((L1[0]>0)and(L1[0]==L2[0])and(L1[0]==L3[0])) {return 1;}
  if((L1[1]>0)and(L1[1]==L2[1])and(L1[1]==L3[1])) {return 1;}
  if((L1[2]>0)and(L1[2]==L2[2])and(L1[2]==L3[2])) {return 1;}
  if((L1[0]>0)and(L1[0]==L1[1])and(L1[0]==L1[2])) {return 1;}
  if((L2[0]>0)and(L2[0]==L2[1])and(L2[0]==L2[2])) {return 1;}
  if((L3[0]>0)and(L3[0]==L3[1])and(L3[0]==L3[2])) {return 1;}
  if((L1[0]>0)and(L1[0]==L2[1])and(L1[0]==L3[2])) {return 1;}
  if((L3[0]>0)and(L3[0]==L2[1])and(L3[0]==L1[2])) {return 1;}
  return 0;
}

void config(){
  while (BlueT.available()) {
    Serial.print(char(BlueT.read())); }
  while (Serial.available()) {
    BlueT.write(char(Serial.read())); }
}
