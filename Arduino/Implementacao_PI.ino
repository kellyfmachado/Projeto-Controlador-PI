#include <Thermistor.h>  //inclusao da biblioteca do thermistor

Thermistor temp(A0);     //utilizando biblioteca do thermistor

//LAMPADA
#define pinLamp 9        //pino lampada

//COOLER
#define pinPWM 3         //pino pwm cooler
#define pinTacometro 2   //pino tacometro
#define tolerancia 0.2
int dutyCycle = 0;

//Tempos
float ultimoTempo = 0;
float tempoInicial = 0;
#define del 5130

//Estabilidade da temperatura
#define amost 50    //Quantidade de amostras para conferir se está em regime permanente
float x [amost];      //Vetor com primeiras amostras
float z [amost];      //Vetor com próximas amostras
int i = 0; 
float media_x = 0;     //Média do primeiro vetor de amostras
float media_z = 0;     //Média do segundo vetor de amostras
float diff_media = 1;  //Diferença entre as médias
unsigned int fezMed = 0;  //Controla se altera o contador ou não

//unsigned int contCool = 0; //Controlar se o Cooler liga, desliga

float u;
float u_ant;
float e;
float e_ant;
float y;
float r;
float d; 
int j = 0;
float controlador_PI(float r, float y);
float media (float a[amost]);

int ref[] = {33, 35, 38};

void setup() {
  //LAMP
  pinMode(pinLamp, OUTPUT);       
  analogWrite(pinLamp, 255);
  
  //COOLER
  pinMode(pinPWM, OUTPUT);
  analogWrite(pinPWM, 0);
  
  //NTC
  Serial.begin(250000);   //INICIALIZA A SERIAL 
  tempoInicial = millis();
}

void loop() {
  float tempoAtual_total = millis();
  float tempoAtual = tempoAtual_total - tempoInicial;
  float temperatura = temp.getTemp();      //VARIÁVEL DO TIPO INTEIRO QUE RECEBE O VALOR DE TEMPERATURA CALCULADO PELA BIBLIOTECA

  if (tempoAtual - ultimoTempo >= del){
    //Armazenamento dos dados nos vetores
    if (i<amost){
      x[i] = temperatura;
    } else if (i>=amost && i<(2*amost)){    
      z[i-amost] = temperatura;    
    } else if (i>=(2*amost)) {   
      media_x = media (x);
      media_z = media (z);
      diff_media = abs(media_z - media_x); 
      i=0;    
      fezMed = 1;                        //Variável de controle para o Cooler
    }
    i++;

  //Ligando e desligando o Cooler

    r = ref[j];
    y = temperatura;
    d = controlador_PI(r,y);
    analogWrite(pinPWM, int(d));
      

    if (diff_media <= tolerancia && j<4 && fezMed == 1){                                    //A definir valor    
      j++;
      fezMed = 0;
    }

    //Imprimindo os dados a serem coletados no monitor serial
    Serial.print(tempoAtual);
    Serial.print(" ");
    Serial.print(r);
    Serial.print(" ");
    Serial.print(temperatura);                              //A definir valor
    Serial.print(" ");
    Serial.print(diff_media);
    Serial.print(" ");
    Serial.println(d);

    ultimoTempo = tempoAtual;
  }
}

//Função para cálculo de média
float media (float a[amost]){
  float med = 0;
  float soma = 0;
  for (int i=0; i < amost; i++ ){
    soma += a[i];    
  }
  med = soma/amost;
  return med;  
}

//Implementação do PI Digital
float controlador_PI(float r, float y){
 e = r-y;
 u = u_ant - 2.5236*e + 1.128*e_ant;
 if (u>255) u=255; // limitador superior
 else if (u<0) u=0; // limitador inferior

 e_ant = e;
 u_ant = u;
 return float(u);
}
