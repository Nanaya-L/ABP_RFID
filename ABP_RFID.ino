//Programa : RFID - Controle de Acesso leitor RFID
//Autor : FILIPEFLOP
 
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define BTN 4
#define BUZZER 3
#define R 5
#define B 6
#define G 7
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int count = 0;
 
String st[20];
bool cadastro = false;
void setup() 
{
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  pinMode(BUZZER,OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(G,OUTPUT);
  
}

int tempo = 400;
void loop() 
{
  lerBotao();

  
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  Serial.print("Cadastro: "+ cadastro);
  
  if (existe(conteudo.substring(1))){
      Serial.print("Acesso concedido: "+ conteudo.substring(1));
     acessoConcedido();
  }else{
    if(cadastro){
      cadastrar(conteudo.substring(1));
      acessoConcedido();
    }else{
      Serial.print("Acesso negado: "+ conteudo.substring(1));
      acessoNegado();
    }
  }
  
}

void acessoConcedido(){
  tone(BUZZER,440,tempo);
  ledRGB(0,0,1);
  delay(1000);
  ledRGB(0,0,0);
}

void acessoNegado(){
  tone(BUZZER,262,tempo);
  ledRGB(1,0,0);
  delay(1000);
  ledRGB(0,0,0);
}

void ledRGB(int r, int b, int g){
  digitalWrite(R,r);
  digitalWrite(G,g);
  digitalWrite(B,b);
}

void cadastrar(String tag){
  Serial.print("Cadastro da tag: "+ tag);
  if(count == 19){
   acessoNegado();
   return;
  }
  
  st[count] = tag;
  count++;
}

bool existe(String tag){
  for(int i = 0; i < 20; i++){
    if(tag == st[i]){
      return true;
    }
  }
  return false;
}
void lerBotao(){
  if (digitalRead(BTN) == HIGH) { // Botão Pressionado;
    cadastro = !cadastro;
    delay(1000);
  }
  ledRGB(0,(int)cadastro,0);
}