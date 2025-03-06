/*   (PET SEMETARY - RAMONES)
Codigo em desenvolvimento....
  ___________________________________________
 |             MAV II - DIP II               |
 |                 iNovaX                    |
 |André_Monção - Julio_Oscar - Rafael_Xavier |
 |___________________________________________|

Incluir Bibliotecas. Adafruit é a do OLED*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definindo o pino do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
//Endereço I2C do display OLED SSD1306

/* Definindo os modos de planta. Escolhi em base de quantidade de umidade, 
sendo cacto baixo, suculentas medias, e sambaias altas. A escolha são apenas
para exemplos, não uso real sendo possivel alterações futuramente. */

#define SUCULENTA 0
#define CACTOS 1
#define SAMAMBAIA 2

// Declaração das funções
//________OLHOS________
void exibirImagem();  // Exibe uma imagem no display
void desenharOlhos();  // Desenha os olhos no display
void MoverPupilasParaAngulo(int angulo);  // Move as pupilas para um ângulo específico
void RestringirPosicaoPupila(int CentroOlhoX, int CentroOlhoY, int &PupilaX, int &PupilaY);  // Restringe a posição das pupilas dentro do globo ocular
void piscar();  // Animação de piscar os olhos
//________EXPRESSOES________
void expressaoCansado();  // Exibe a expressão de cansaço
void expressaoTriste();  // Exibe a expressão de tristeza
void expressaoBase();  // Exibe a expressão base (olhos normais)
void expressaoDesconfiado();  // Exibe a expressão de desconfiança
void expressaoFeliz();  // Exibe a expressão de felicidade
void definirExpressao();  // Define a expressão com base na umidade do solo
//_________BOTOES___________
void exibirFaixaUmidade();  // Exibe a faixa de umidade atual e recomendada
void exibirModoAtual();  // Exibe o modo de planta atual
void selecionarModo();  // Permite ao usuário selecionar o modo de planta
void alternarModo();  // Alterna entre os modos de planta
void confirmarModo();  // Confirma a seleção do modo de planta
void resetarTempoTela();  // Reseta o tempo de exibição na tela
void aguardarEntrada();  // Aguarda a entrada do usuário
//17 voids? vou tentar usar um cabeçalho para diminuir isto.
// Definindo o pino para o sensor de umidade
const int PINO_POTENCIOMETRO = A0; // Simulador não tem o sensor HW-080
int umidade;
int umidadeMinima = 40; // Umidade mínima ideal
int umidadeMaxima = 60; // Umidade máxima ideal

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int TamanhoPupila = 10;  // Tamanho das pupilas
int RaioOcular = 25;  // Raio do globo ocular

// Posição dos olhos
int CentroOlhoX1 = SCREEN_WIDTH / 1.3;
int CentroOlhoY1 = SCREEN_HEIGHT / 2;
int CentroOlhoX2 = SCREEN_WIDTH / 4;
int CentroOlhoY2 = SCREEN_HEIGHT / 2;

// Posição das pupilas
int PupilaX1 = CentroOlhoX1;
int PupilaY1 = CentroOlhoY1;
int PupilaX2 = CentroOlhoX2;
int PupilaY2 = CentroOlhoY2;

// Variáveis para botões
const int BOTAO1 = 2;
const int BOTAO2 = 3;
const int BOTAO3 = 4;
int modoAtual = SUCULENTA; // 0: suculenta, 1: cactos, 2: samambaia
bool selecionandoModo = false;
unsigned long tempoTela = 0;

// Variáveis para controle de tempo não bloqueante
unsigned long tempoAnterior = 0;
const unsigned long intervaloAnimacao = 500; // Intervalo para animação (500ms)

// Tumulo
const unsigned char epd_bitmap_tumulo [] PROGMEM = {
	// Aqui vai o código hexadecimal da imagem
    	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xff, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0e, 0x03, 0xce, 0x01, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x03, 0xce, 0x01, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x7b, 0xce, 0x31, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x7b, 0xce, 0x31, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x03, 0xce, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x07, 0xce, 0x3e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x07, 0xce, 0x3e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x7b, 0xce, 0x3e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x7b, 0xce, 0x3e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xfe, 0x01, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xf0, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xf0, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf0, 0x7c, 0x7e, 0x3f, 0x9f, 0x38, 0x79, 0xe7, 0xf0, 0x7c, 0x4e, 0x0e, 0x64, 0x3f, 0xff, 
	0xff, 0xe0, 0x38, 0x3c, 0x1f, 0x9f, 0x30, 0x39, 0xe7, 0xe0, 0x38, 0x0c, 0x06, 0x60, 0x1f, 0xff, 
	0xff, 0xe7, 0x33, 0x99, 0xcf, 0xce, 0x63, 0x19, 0xe7, 0xe7, 0x31, 0x8c, 0xe6, 0x63, 0x9f, 0xff, 
	0xff, 0xe1, 0xf0, 0x18, 0x0f, 0xce, 0x67, 0x99, 0xe7, 0xfc, 0x33, 0xcf, 0x86, 0x67, 0x9f, 0xff, 
	0xff, 0xf0, 0x70, 0x18, 0x0f, 0xe4, 0xe7, 0x99, 0xe7, 0xf0, 0x33, 0xce, 0x06, 0x67, 0x9f, 0xff, 
	0xff, 0xfc, 0x33, 0xf9, 0xff, 0xe4, 0xe7, 0x99, 0xe7, 0xe3, 0x33, 0xcc, 0x66, 0x67, 0x9f, 0xff, 
	0xff, 0xe7, 0x31, 0x98, 0xcf, 0xe0, 0xe3, 0x19, 0xc7, 0xe7, 0x31, 0x8c, 0xe6, 0x67, 0x9f, 0xff, 
	0xff, 0xe0, 0x38, 0x3c, 0x1f, 0xf1, 0xf0, 0x38, 0x07, 0xe0, 0x38, 0x0c, 0x06, 0x67, 0x9f, 0xff, 
	0xff, 0xf0, 0x7c, 0x7e, 0x3f, 0xf1, 0xf8, 0x7c, 0x27, 0xf0, 0x9c, 0x4e, 0x12, 0x67, 0x9f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xcf, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

//Hexadecimal do tumulo caso o valor seja 0 ou 100%

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Loop infinito caso o display falhe
  }
  
//Usei 3 botoes pois tive dificuldade com 2 botoes, na hora de trocar o modo ele bugava.
  pinMode(BOTAO1, INPUT_PULLUP); //Mostra a % do sensor
  pinMode(BOTAO2, INPUT_PULLUP); //Mostra o Modo Atual
  pinMode(BOTAO3, INPUT_PULLUP); //Troca o Modo da planta. 

  // Inicializando os olhos
  display.clearDisplay(); //Limpa o display
  display.setTextSize(1); // Reduzindo o tamanho da fonte
  display.setTextColor(WHITE); //White aqui significa que o LED vai acender
  display.display(); //Exibe o display
  expressaoBase();
}

///////////////////////////////VOID LOOP///////////////////////////////////////////////

void loop() {
//Escolhi usar millis para que os botoes sejam instantaneos e nao tenham que esperar o fim do delay
  static unsigned long tempoAnterior = 0; // Armazena o tempo da última execução
  static int etapa = 0; // Controla a etapa atual da animação
  static int angulos[3]; // Armazena os ângulos das pupilas
  static int indiceAngulo = 0; // Controla o índice do ângulo atual

  unsigned long tempoAtual = millis(); // Tempo atual

  // Ler a umidade do solo
  int leitura = analogRead(PINO_POTENCIOMETRO);
  umidade = map(leitura, 0, 1023, 0, 100);

  // Verificar interações com os botões
  if (digitalRead(BOTAO1) == LOW) {
    exibirFaixaUmidade();
    resetarTempoTela();
    aguardarEntrada();
  }

  if (digitalRead(BOTAO2) == LOW) {
    exibirModoAtual();
    resetarTempoTela();
    aguardarEntrada();
  }

  if (digitalRead(BOTAO3) == LOW) {
    selecionarModo();
  }

  // Lógica das animações e expressões
  // Muito MasterMind usar uma switch case para executar as animações, eu estava com muita dificuldade nessa parte, vlw Otavio...

  switch (etapa) {
    case 0: // Exibir a expressão base
      display.clearDisplay();
      expressaoBase();
      if (tempoAtual - tempoAnterior >= 1000) { // Espera 1 segundo
        tempoAnterior = tempoAtual;
        etapa = 1; // Passa para a próxima etapa
      }
      break;

    case 1: // Exibir a expressão com base na umidade
      definirExpressao(); // Chama a função que define a expressão com base na umidade
      if (tempoAtual - tempoAnterior >= 1000) { // Espera 1 segundo
        tempoAnterior = tempoAtual;
        etapa = 2; // Passa para a próxima etapa
      }
      break;

    case 2: // Gerar ângulos aleatórios para as pupilas
      for (int i = 0; i < 3; i++) {
        angulos[i] = random(0, 360);
      }
      etapa = 3; // Passa para a próxima etapa
      break;

    case 3: // Mover as pupilas para os ângulos gerados
      if (indiceAngulo < 3) {
        if (tempoAtual - tempoAnterior >= 500) { // Espera 500ms para cada movimento
          tempoAnterior = tempoAtual;
          MoverPupilasParaAngulo(angulos[indiceAngulo]);
          indiceAngulo++;
        }
      } else {
        indiceAngulo = 0; // Reinicia o índice dos ângulos
        etapa = 4; // Passa para a próxima etapa
      }
      break;

    case 4: // Piscar os olhos
      piscar();
      if (tempoAtual - tempoAnterior >= 500) { // Espera 500ms após piscar
        tempoAnterior = tempoAtual;
        etapa = 0; // Volta para a primeira etapa (expressão base)
      }
      break;
  }
}
/* Em resumo o Switch e Case são usados para dividir a animação em etapas. Cada case representa uma etapa da animação
(expressão base, expressão com base da umidade, piscar, etc...) 
 O tempo de Cada etapa é controlado pelo "millis" o que garante que o programa não fique bloqueado como seria com "delay".
Após a ultima etapa o ciclo reinicia, criando uma animação continua :D
 Eu tentei usar Interrupt e Flags para os botoes mas travava as animações.*/

///////////////////////////////////FIM VOID LOOP//////////////////////////////////////

// Exibir a Imagem do BitMap quando o valor for = 0% ou 100%
void exibirImagem() {
  display.clearDisplay(); // Limpa o display
  display.drawBitmap(0, 0, epd_bitmap_tumulo, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE); // Exibe a imagem
  display.display(); // Atualiza o display
}

// Função para definir a expressão com base na umidade
void definirExpressao() {
  if (umidade == 0 || umidade == 100) { // Se a umidade for exatamente 0% ou 100%
    exibirImagem(); // Exibe a imagem do bitmap
  } else if (umidade < umidadeMinima) {
    expressaoTriste(); // Exibe expressão triste se a umidade estiver baixa
  } else if (umidade >= umidadeMinima && umidade <= umidadeMaxima) {
    expressaoFeliz(); // Exibe expressão feliz se a umidade estiver ideal
  } else {
    expressaoCansado(); // Exibe expressão cansada se a umidade estiver alta
  }
  display.display(); // Atualiza o display com a expressão
}

//////////////////////////PUPILAS///////////////////////////////////////////////////
void MoverPupilasParaAngulo(int angulo) {
  int distancia = random(0, RaioOcular - TamanhoPupila);

  // Calcular a posição da pupila com base no ângulo (os valores estão antes do Setup)
  int novaPupilaX1 = CentroOlhoX1 + distancia * cos(radians(angulo));
  int novaPupilaY1 = CentroOlhoY1 + distancia * sin(radians(angulo));
  int novaPupilaX2 = CentroOlhoX2 + distancia * cos(radians(angulo));
  int novaPupilaY2 = CentroOlhoY2 + distancia * sin(radians(angulo));

  // Suavizar o movimento do globo ocular
  int deslocamentoX1 = (novaPupilaX1 - PupilaX1) / 3;
  int deslocamentoY1 = (novaPupilaY1 - PupilaY1) / 3;
  int deslocamentoX2 = (novaPupilaX2 - PupilaX2) / 3;
  int deslocamentoY2 = (novaPupilaY2 - PupilaY2) / 3;

  // Movimentar suavemente o globo ocular para dar uma animação com maior fluidez
  CentroOlhoX1 += deslocamentoX1;
  CentroOlhoY1 += deslocamentoY1;
  CentroOlhoX2 += deslocamentoX2;
  CentroOlhoY2 += deslocamentoY2;

  // Atualizar as posições das pupilas
  PupilaX1 = novaPupilaX1;
  PupilaY1 = novaPupilaY1;
  PupilaX2 = novaPupilaX2;
  PupilaY2 = novaPupilaY2;

  // Restringir as posições para não ultrapassar os limites do globo ocular
  RestringirPosicaoPupila(CentroOlhoX1, CentroOlhoY1, PupilaX1, PupilaY1);
  RestringirPosicaoPupila(CentroOlhoX2, CentroOlhoY2, PupilaX2, PupilaY2);

  display.clearDisplay();
  display.fillCircle(CentroOlhoX1, CentroOlhoY1, RaioOcular, WHITE);
  display.fillCircle(CentroOlhoX2, CentroOlhoY2, RaioOcular, WHITE);
  display.fillCircle(PupilaX1, PupilaY1, TamanhoPupila, BLACK);
  display.fillCircle(PupilaX2, PupilaY2, TamanhoPupila, BLACK);
  display.display();
}

void RestringirPosicaoPupila(int CentroOlhoX, int CentroOlhoY, int &PupilaX, int &PupilaY) {
  if (PupilaX < CentroOlhoX - RaioOcular + TamanhoPupila) PupilaX = CentroOlhoX - RaioOcular + TamanhoPupila;
  if (PupilaX > CentroOlhoX + RaioOcular - TamanhoPupila) PupilaX = CentroOlhoX + RaioOcular - TamanhoPupila;
  if (PupilaY < CentroOlhoY - RaioOcular + TamanhoPupila) PupilaY = CentroOlhoY - RaioOcular + TamanhoPupila;
  if (PupilaY > CentroOlhoY + RaioOcular - TamanhoPupila) PupilaY = CentroOlhoY + RaioOcular - TamanhoPupila;
}
/////////////////////////////////////FIM PUPILAS////////////////////////////////////////

////////////////////////////////////EXPRESSOES///////////////////////////////////////////
void piscar() {
//Ela segue 3 fases para dar o efeito dos olhos se fechando
  // Fase 1: Apagar linhas mantendo de 16 a 48
  for (int i = 1; i <= 32; i++) {
    if (i >= 16 && i <= 48) continue;
    display.drawLine(0, i - 1, SCREEN_WIDTH - 1, i - 1, BLACK);
    display.drawLine(0, SCREEN_HEIGHT - i, SCREEN_WIDTH - 1, SCREEN_HEIGHT - i, BLACK);
  }
  display.display();
  delay(5);

  // Fase 2: Apagar linhas mantendo de 25 a 39
  for (int i = 1; i <= 32; i++) {
    if (i >= 25 && i <= 39) continue;
    display.drawLine(0, i - 1, SCREEN_WIDTH - 1, i - 1, BLACK);
    display.drawLine(0, SCREEN_HEIGHT - i, SCREEN_WIDTH - 1, SCREEN_HEIGHT - i, BLACK);
  }
  display.display();
  delay(5);

  // Fase 3: Apagar linhas mantendo de 32
  for (int i = 1; i <= 32; i++) {
    if (i >= 32 && i <= 32) continue;
    display.drawLine(0, i - 1, SCREEN_WIDTH - 1, i - 1, BLACK);
    display.drawLine(0, SCREEN_HEIGHT - i, SCREEN_WIDTH - 1, SCREEN_HEIGHT - i, BLACK);
  }
  display.display();
  delay(5);

  // Limpar tela antes de acender novamente
  display.clearDisplay();
  display.display();
  delay(5);
}

//Essa criei por acaso
void expressaoDesconfiado() {
  for (int i = 0; i <= 26; i++) {
    display.drawLine(0, i, SCREEN_WIDTH - 1, i, BLACK);
  }
  int x1 = 0, y1 = SCREEN_HEIGHT - 1;
  int x2 = SCREEN_WIDTH - 1, y2 = SCREEN_HEIGHT - 1;
  int x3 = SCREEN_WIDTH / 2, y3 = SCREEN_HEIGHT / 2;

  display.fillTriangle(x1, y1, x2, y2, x3, y3, BLACK);
  display.display();
}

void expressaoTriste() {
  int x1 = 0, y1 = 0;
  int x2 = SCREEN_WIDTH / 2, y2 = 0;
  int x3 = 0, y3 = SCREEN_HEIGHT / 2;

  display.fillTriangle(x1, y1, x2, y2, x3, y3, BLACK);
//Defini dois triangulos para dar a impressao de um olhar triste
  int x4 = SCREEN_WIDTH - 1, y4 = 0;
  int x5 = SCREEN_WIDTH / 2, y5 = 0;
  int x6 = SCREEN_WIDTH - 1, y6 = SCREEN_HEIGHT / 2;

  int x_centro = SCREEN_WIDTH / 2;
  int y_centro = SCREEN_HEIGHT - 5;
  int raio = 8;
  int espessura = 2;
// Acrescentei uma boca encurvada para dar uma enfase na animação
  for (int i = 200; i <= 340; i++) {
    for (int j = -espessura; j <= espessura; j++) {
      int x = x_centro + raio * cos(i * PI / 180);
      int y = y_centro + raio * sin(i * PI / 180);
      display.drawPixel(x, y, WHITE);
    }
  }

  display.fillTriangle(x4, y4, x5, y5, x6, y6, BLACK);
  display.display();
}
//O olhar para frente.
void expressaoBase() {
  CentroOlhoX1 = SCREEN_WIDTH / 1.3;
  CentroOlhoY1 = SCREEN_HEIGHT / 2;
  CentroOlhoX2 = SCREEN_WIDTH / 4;
  CentroOlhoY2 = SCREEN_HEIGHT / 2;

  PupilaX1 = CentroOlhoX1;
  PupilaY1 = CentroOlhoY1;
  PupilaX2 = CentroOlhoX2;
  PupilaY2 = CentroOlhoY2;

  display.clearDisplay();
  display.fillCircle(CentroOlhoX1, CentroOlhoY1, RaioOcular, WHITE);
  display.fillCircle(CentroOlhoX2, CentroOlhoY2, RaioOcular, WHITE);
  display.fillCircle(PupilaX1, PupilaY1, TamanhoPupila, BLACK);
  display.fillCircle(PupilaX2, PupilaY2, TamanhoPupila, BLACK);
  display.display();
}
// Meu olhar
void expressaoCansado() {
  for (int i = 0; i <= 26; i++) {
    display.drawLine(0, i, SCREEN_WIDTH - 1, i, BLACK);
  }
  display.display();
}

void expressaoFeliz() {
  int x1 = SCREEN_WIDTH / 4;
  int y1 = SCREEN_HEIGHT - 1;
  int raio = 45;
//Precisei incluir 2 circulos para cobrir a os olhos e dar a impressao do olho levantado, acrescentei tambem uma boca
  display.fillCircle(x1, y1, raio, BLACK);

  int x2 = SCREEN_WIDTH / 1.3;
  int y2 = SCREEN_HEIGHT - 1;

  display.fillCircle(x2, y2, raio, BLACK);

  int x_centro1 = SCREEN_WIDTH / 2;
  int y_centro1 = SCREEN_HEIGHT - 20;
  int raio2 = 10;
  int espessura = 1;
//Boca
  for (int i = 20; i <= 160; i++) {
    for (int j = -espessura; j <= espessura; j++) {
      int x = x_centro1 + (raio2 + j) * cos(i * PI / 180);
      int y = y_centro1 + (raio2 + j) * sin(i * PI / 180);
      display.drawPixel(x, y, WHITE);
    }
  }
  display.display();
}

/////////////////////////////VOIDS BOTOES//////////////////////////////////////

void exibirFaixaUmidade() {
  int leitura = analogRead(PINO_POTENCIOMETRO);
  int porcentagem = map(leitura, 0, 1023, 0, 100);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.print("Umidade Atual: ");
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.print(porcentagem);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(10, 40);
  display.print("Recomendado: ");
  display.setCursor(10, 55);

  if (modoAtual == SUCULENTA) {
    display.print("20 a 40%");
  } else if (modoAtual == CACTOS) {
    display.print("10 a 20%");
  } else {
    display.print("40 a 70%");
  }
  display.display();
}

void exibirModoAtual() {
  display.clearDisplay();
  display.setCursor(30, 25);
  if (modoAtual == SUCULENTA) {
    display.print("Suculenta");
  } else if (modoAtual == CACTOS) {
    display.print("Cactos");
  } else {
    display.print("Samambaia");
  }
  display.display();
}

void selecionarModo() {
  selecionandoModo = true;
  resetarTempoTela(); // Para não ficar travado na tela esperando uma ação
  while (millis() - tempoTela < 3000) {
    display.clearDisplay();
    display.setCursor(30, 25);
    if (modoAtual == SUCULENTA) {
      display.print("Suculenta");
    } else if (modoAtual == CACTOS) {
      display.print("Cactos");
    } else {
      display.print("Samambaia");
    }
    display.display();

    if (digitalRead(BOTAO3) == LOW) {
      alternarModo();
      resetarTempoTela(); //Antes dessa opção ele estava somando o tempo ao apertar os botões.
      delay(50);
      display.clearDisplay();
      delay(50);
    }
    if (digitalRead(BOTAO2) == LOW) {
      confirmarModo();
      return;
    }
  }
  confirmarModo();
}

void alternarModo() {
  if (modoAtual == SUCULENTA) {
    modoAtual = CACTOS;
  } else if (modoAtual == CACTOS) {
    modoAtual = SAMAMBAIA;
  } else {
    modoAtual = SUCULENTA;
  }
  exibirModoAtual();
}

void confirmarModo() {
  display.clearDisplay();
  display.setCursor(40, 25);
  display.print("OK");
  display.display();
  delay(1000);
  expressaoBase();
}

void resetarTempoTela() {
  tempoTela = millis();
}

/* A função abaixo aguarda 5 segundos por uma interação do usuário. Se o BOTAO2 for pressionado, exibe o 
modo atual e aguarda outra interação por 5 segundos. Se o BOTAO1 ou BOTAO3 for pressionado durante esse tempo, 
executa ações específicas e encerra a função. E se nenhum botão for pressionado, volta para a tela inicial após 5 segundos.
Ela é necessaria para voltar a tela inicial */
void aguardarEntrada() {
  while (millis() - tempoTela < 3000) {
    if (digitalRead(BOTAO2) == LOW) {
      exibirModoAtual();
      resetarTempoTela();
      while (millis() - tempoTela < 3000) {
        if (digitalRead(BOTAO1) == LOW) {
          expressaoBase();
          return;
        }
        if (digitalRead(BOTAO3) == LOW) {
          selecionarModo();
          return;
        }
      }
      expressaoBase();
      return;
    }
  }
  expressaoBase();
}

//Este codigo ainda está sujeito a alterações!!! Irei deixar os artigos de referencias em outra pasta para consultas.
