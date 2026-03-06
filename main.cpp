/*
* Sistema de alarme residencial por MATHEUS RODRIGUES QUARESMA
*
*/

#include <LiquidCrystal.h> //inclui a biblioteca do LCD 
#include <Keypad.h>

#define sirene 13
#define buzzer 10
#define sensorPIR 1
#define gLed 8
#define rLed 9
#define yLed 0

long duration;
//int distance, initialDistance, currentDistance, i;
int leituraSensor = LOW;
int screenOffMsg = 0;
String password="1234";
String tempPassword;
boolean activated = false; //Estado do alarme
boolean isActivated = false;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword; //Estado do alarme apos senha inserida com o alarme disparado
boolean passChangeMode = false;
boolean passChanged = false;
boolean desactivate = false;

const byte ROWS = 4; //quatro linhas
const byte COLS = 4; //quatro colunas
char keypressed;
//define os numeros e as letras do teclado
char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, A5, A4}; //Pinagem das linhas do teclado
byte colPins[COLS] = {A0, A1, A2, A3}; //Pinagem das colunas do teclado

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

void setup() { 
  lcd.begin(16,2); 
  pinMode(sirene, OUTPUT); //Seta a sirene como saída
  pinMode(buzzer, OUTPUT); //Seta o buzzer como saída
  pinMode(sensorPIR, INPUT); //Seta o sensor como saída
  pinMode(gLed, OUTPUT); //Seta o LED verde como saída
  pinMode(rLed, OUTPUT); //Seta o LED vermelho como saída
  pinMode(yLed, OUTPUT); //Seta o LET amarelo como saída
}

void loop() {
  if (activateAlarm) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Alarme sera");
    lcd.setCursor(0,1);
    lcd.print("  ativado em");
   
    int countdown = 9; //Ativa o alarme após 9 segundos da confirmação
    while (countdown != 0) {
      lcd.setCursor(13,1);
      lcd.print(countdown);
      countdown--;
      tone(buzzer, 1500, 100);
      delay(1000);
      digitalWrite(gLed, HIGH);
      delay(500);
      digitalWrite(gLed, LOW);
      delay(500);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("---- Alarme ----");
    lcd.setCursor(0,1);
    lcd.print("--- ativado! ---");
    digitalWrite(rLed, LOW);
    digitalWrite(yLed, LOW);
    digitalWrite(gLed, HIGH);
    activateAlarm = false;
    alarmActivated = true; 
  }
  
  
  if(alarmActivated){
  	keypressed = myKeypad.getKey(); 
    if(keypressed =='D'){ //Se D for pressionado com o alarme operante,
    	int k=6;		  //solicita a senha para desativar o alarme
  		tempPassword = "";
        activated = true;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Desativar: ");
      	while(activated) {
      		keypressed = myKeypad.getKey();
        	if (keypressed != NO_KEY){
          		if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
              		keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
              		keypressed == '8' || keypressed == '9' ) {
            		tempPassword += keypressed;
            		lcd.setCursor(k,1);
            		lcd.print("*");
            		k++;
          		}
        	}
        	if (k > 10 || keypressed == '#') { //Se # for pressionada, limpa a senha digitada
          		tempPassword = "";
                k=6;
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Desativar:");
        	}
        	if ( keypressed == '*') {  //Se * for pressionado, confirma a senha digitada
          		if ( tempPassword == password ) {
                 	lcd.clear();
                  	lcd.print("DESATIVANDO...");
                  	tone(buzzer, 1000, 200);
                    digitalWrite(gLed, HIGH);
                    delay(250);
                    digitalWrite(gLed, LOW);
                    delay(250);
                	activated = false;
                    alarmActivated = false;
                    noTone(buzzer);
                    digitalWrite(rLed, LOW);
                    digitalWrite(yLed, HIGH);
                    screenOffMsg = 0;
          		}
          		else if (tempPassword != password) { //Se a senha estiver incorreta,
                    lcd.clear();					 //apresenta mensagem de erro e consiste
                    lcd.setCursor(0,0);
                    lcd.print("   Incorreto!");
                    lcd.setCursor(0,1);
                    lcd.print("Tente novamente");
                    delay(2000);
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Desativar:");
          	}
        }    
      }

    }
  }
  
    
 
  
  if (alarmActivated == true){ //Se true, define o alarme como ativado
      leituraSensor = digitalRead(sensorPIR);
      if (leituraSensor == HIGH) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Alarme sera");
        lcd.setCursor(0,1);
        lcd.print("disparado em");
        int countdown = 30; //Ativa o alarme após 9 segundos da confirmação
    	while (countdown != 0) {
          lcd.setCursor(13,1);
          lcd.print(countdown);
          countdown--;
          digitalWrite(gLed, HIGH);
          delay(500);
          digitalWrite(gLed, LOW);
          delay(500);
                   keypressed = myKeypad.getKey(); 
              if(keypressed =='D'){ //Se D for pressionado com o alarme operante,
                  int k=6;		  //solicita a senha para desativar o alarme
                  tempPassword = "";
                  activated = true;
                  //lcd.clear();
                  //lcd.setCursor(0,0);
                  //lcd.print("Desativar: ");
                  while(activated) {
                      keypressed = myKeypad.getKey();
                      if (keypressed != NO_KEY){
                          if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                              keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                              keypressed == '8' || keypressed == '9' ) {
                              tempPassword += keypressed;
                              lcd.setCursor(k,1);
                              lcd.print("*");
                              k++;
                          }
                      }
                      if (k > 10 || keypressed == '#') { //Se # for pressionada, limpa a senha digitada
                          tempPassword = "";
                          k=6;
                          lcd.clear();
                          lcd.setCursor(0,0);
                          lcd.print("Desativar:");
                      }
                      if ( keypressed == '*') {  //Se * for pressionado, confirma a senha digitada
                          if ( tempPassword == password ) {
                              lcd.clear();
                              lcd.print("DESATIVANDO...");
                              tone(buzzer, 1000, 200);
                              digitalWrite(gLed, HIGH);
                              delay(250);
                              digitalWrite(gLed, LOW);
                              delay(250);
                              activated = false;
                              alarmActivated = false;
                              noTone(buzzer);
                              digitalWrite(rLed, LOW);
                              digitalWrite(yLed, HIGH);
                              screenOffMsg = 0;
                          }
                          else if (tempPassword != password) { //Se a senha estiver incorreta,
                              lcd.clear();					 //apresenta mensagem de erro e consiste
                              lcd.setCursor(0,0);
                              lcd.print("   Incorreto!");
                              lcd.setCursor(0,1);
                              lcd.print("Tente novamente");
                              delay(2000);
                              lcd.clear();
                              lcd.setCursor(0,0);
                              lcd.print("Desativar:");
                      }
                  }    
                }

              }
    	}
        tone(sirene, 1000); // Manda um sinal de som de 1KHz
        lcd.clear();
        enterPassword();
      }
    }

  if (!alarmActivated) {
    if (screenOffMsg == 0 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Ativar");
      lcd.setCursor(0,1);
      lcd.print("B - Mudar senha");
      digitalWrite(yLed, HIGH);
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();    
     if (keypressed =='A'){  //Se A for pressionado, ativa o alarme
      tone(buzzer, 1000, 200);
      activateAlarm = true;            
    }
    else if (keypressed =='B') { //Se B for pressionado, troca a senha
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Senha atual:"); //Solicita a senha atual
      passChangeMode = true;
      passChanged = true;   
      while(passChanged) {      
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tempPassword += keypressed;
         lcd.setCursor(i,1);
         lcd.print("*");
         i++;
         tone(buzzer, 2000, 100);
        }
      }
      if (i > 5 || keypressed == '#') { //Se # for pressionada, limpa a senha digitada
        tempPassword = "";
        i=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Senha atual:");
      }
      if ( keypressed == '*') { //Se * for pressionado, confirma a senha digitada
        i=1;
        tone(buzzer, 2000, 100);
        if (password == tempPassword) {
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Nova senha:"); //Solicita a nova senha
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') { //Se # for pressionada, limpa a senha digitada
              tempPassword = "";
              i=1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Nova senha:");
            }
            if ( keypressed == '*') { //Se * for pressionada, confirma a senha digitada
              i=1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false;
              passChanged = false;
              screenOffMsg = 0;
            }            
          }
        }
      }
    }    
   }
 }
}

void enterPassword() { //Função que consiste em pedir a senha
  int k=6;			   //para desativar o alarme disparado
  tempPassword = "";
  activated = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" *** ALARME *** ");
  lcd.setCursor(0,1);
  lcd.print("SENHA: ");
  digitalWrite(gLed, LOW);
  digitalWrite(rLed, HIGH);
      while(activated) {
      keypressed = myKeypad.getKey();
        if (keypressed != NO_KEY){
          if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
              keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
              keypressed == '8' || keypressed == '9' ) {
            tempPassword += keypressed;
            lcd.setCursor(k,1);
            lcd.print("*");
            k++;
          }
        }
        if (k > 10 || keypressed == '#') { //Se # for pressionada, limpa a senha digitada
          tempPassword = "";
          k=6;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" *** ALARME *** ");
          lcd.setCursor(0,1);
          lcd.print("SENHA:");
          digitalWrite(gLed, LOW);
          digitalWrite(rLed, HIGH);
        }
        if ( keypressed == '*') { //Se * for pressionado, confirma a senha digitada
          if ( tempPassword == password ) {
            activated = false;
            alarmActivated = false;
            noTone(sirene);
            digitalWrite(rLed, LOW);
            digitalWrite(yLed, HIGH);
            screenOffMsg = 0;
          }
          else if (tempPassword != password) { //Se a senha estiver incorreta,
            lcd.clear();					   //apresenta mensagem de erro e consiste
            lcd.setCursor(0,0);
            lcd.print("   Incorreto!");
            lcd.setCursor(0,1);
            lcd.print("Tente novamente");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" *** ALARME *** ");
            lcd.setCursor(0,1);
            lcd.print("SENHA:");
          }
        }    
      }
}