float vetCorrente[300];

const int releInv = 5;
const int releCont = 4;

int ativaSolar = 0;
int op = 0;

float correnteLimiteMin = 8.0;
float correnteLimiteMax = 9.5;

float correnteReativaMin = 0.1;
float correnteReativaMax = 0.3;

float tensaoBateriaMin = 22; // igual a 22.4v do controlador 
float tensaoBateriaMax = 24.6; // igual a 25v do controlador (controlador sempre 0,4v a mais) // 24.6
 
const int pinoSensorBateria = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
const int pinoSensorNoite = A1; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
const int pinoCorrente = A2; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
 
float tensaoEntradaBateria = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float bateria = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR

float tensaoEntradaNoite = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float noite = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR
 
float valorR1 = 30000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float valorR2 = 7500.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO

float vR1 = 10000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float vR2 = 1000.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO

int leituraSensorBateria = 0; //VARIÁVEL PARA ARMAZENAR A LEITURA DO PINO ANALÓGICO
int leituraSensorNoite = 0; //VARIÁVEL PARA ARMAZENAR A LEITURA DO PINO ANALÓGICO
 
void setup(){
 
   pinMode(pinoCorrente, INPUT);
   pinMode(pinoSensorBateria, INPUT); //DEFINE O PINO COMO ENTRADA
   pinMode(pinoSensorNoite, INPUT); //DEFINE O PINO COMO ENTRADA
   pinMode(releInv, OUTPUT);
   pinMode(releCont, OUTPUT);
   Serial.begin(9600); //INICIALIZA A SERIAL
}
void loop(){
    leituraSensorBateria = analogRead(pinoSensorBateria); //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
    leituraSensorNoite = analogRead(pinoSensorNoite); //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
   //Serial.println(leituraSensorBateria);
   
    tensaoEntradaNoite = (leituraSensorNoite * 5.0) / 1024.0; //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
    noite = tensaoEntradaNoite / (valorR2/(valorR1+valorR2)); //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR

    
    tensaoEntradaBateria = (leituraSensorBateria * 5.0) / 1024.0; //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
    bateria = tensaoEntradaBateria / (vR2/(vR1+vR2)); //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
   
//    Serial.print("Tensão Baterias : "); //IMPRIME O TEXTO NA SERIAL
//    Serial.print(bateria,2); //IMPRIME NA SERIAL O VALOR DE TENSÃO DC MEDIDA E LIMITA O VALOR A 2 CASAS DECIMAIS
//    Serial.println("V"); //IMPRIME O TEXTO NA SERIAL

//    Serial.print("Tensão Load : "); //IMPRIME O TEXTO NA SERIAL
//    Serial.print(noite,2); //IMPRIME NA SERIAL O VALOR DE TENSÃO DC MEDIDA E LIMITA O VALOR A 2 CASAS DECIMAIS
//    Serial.println("V"); //IMPRIME O TEXTO NA SERIAL

    float corrente = mede_corrente();

//    Serial.print("Corrente = ");
//    Serial.print(corrente);
//    Serial.println(" A");
    Serial.print("ativaSolar  ");
    Serial.println(ativaSolar);
//    Serial.println("**************");
    Serial.print("Conrrente:  ");
    Serial.println(corrente,3);
//    Serial.println("**************");
    Serial.print("Noite:  ");
    Serial.println(noite,1);
//    Serial.println("**************");
    Serial.print("Bateria:  ");
    Serial.println(bateria,1);
    Serial.print("Operacao:  ");
    Serial.println(op);
    Serial.print("Reler Contatora:  ");
    Serial.println(digitalRead(releCont));
    Serial.print("Rele Inversor:  ");
    Serial.println(digitalRead(releInv));
    Serial.println("**************");
    
    
    delay(500); //INTERVALO DE 3000 MILISSEGUNDOS = 3 segundos
    
    if (noite == 0 && corrente <= correnteLimiteMin && corrente >= correnteReativaMax &&  bateria >= tensaoBateriaMax && ativaSolar == 0){
      ativarSolar();
      Serial.println("1");
      op = 1;
      
    }
    if (noite > 0 && bateria >= tensaoBateriaMax && corrente <= correnteLimiteMin && corrente >= correnteReativaMax && ativaSolar == 0){
      ativarSolar();
      Serial.println("2");
      op = 2;
    }

//        desativar
        
    if (noite == 0 && (corrente >= correnteLimiteMax || corrente <= correnteReativaMin || bateria <= tensaoBateriaMin) && ativaSolar == 1){
      desativarSolar();
      Serial.println("3");
      op = 3;
    }
    if (noite > 0 && (corrente >= correnteLimiteMax || corrente <= correnteReativaMin || bateria <= tensaoBateriaMin) && ativaSolar == 1){
      desativarSolar();
      Serial.println("4");
      op = 4;
    }
       
}



 float mede_corrente(){
   float maior_Valor = 0;
   float valor_Corrente = 0;  
   
   for(int i = 0; i < 300; i++){
    vetCorrente[i] = analogRead(pinoCorrente);
    delayMicroseconds(600);
   }  
   
   for(int i = 0; i < 300; i++){
    if(maior_Valor < vetCorrente[i]){
      maior_Valor = vetCorrente[i];
    }
   }  
   
   maior_Valor = maior_Valor * 0.004882812;
   valor_Corrente = maior_Valor - 2.5;
   valor_Corrente = valor_Corrente * 1000;
   valor_Corrente = valor_Corrente / 66;         //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
   valor_Corrente = valor_Corrente / 1.41421356;
   
   return valor_Corrente;
 }



void ativarSolar(){
  // releInv HIGH
  digitalWrite(releInv, HIGH);  
  //delay(5000)
  //releCont HIGH
  digitalWrite(releCont, HIGH); 
  ativaSolar = 1;
  
}


void desativarSolar(){
  // releCont low
  digitalWrite(releCont, LOW); 
  delay(5000);
  //releInv low
  digitalWrite(releInv, LOW); 
  ativaSolar = 0;
  
}
