/*   (PET SEMETARY - RAMONES)
Código em desenvolvimento....
  ___________________________________________
 |             MAV II - DIP II               |
 |                 iNovaX                    |
 |André_Monção - Julio_Oscar - Rafael_Xavier |
 |___________________________________________|
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <AnimacaoOlhos.h> // Incluindo a biblioteca de animação dos olhos

// Pinos SPI para o display ST7789
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST  -1  // ou use um pino real se estiver conectado

#define SUCULENTA 0
#define CACTOS 1
#define SAMAMBAIA 2

const int PINO_POTENCIOMETRO = A0;
int umidade;
int umidadeMinima = 40;
int umidadeMaxima = 60;

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
AnimacaoOlhos animacao(display);

const int BOTAO1 = 2;
const int BOTAO2 = 3;
const int BOTAO3 = 4;
int modoAtual = SUCULENTA;
bool selecionandoModo = false;
unsigned long tempoTela = 0;

unsigned long tempoAnterior = 0;
const unsigned long intervaloAnimacao = 500;

void setup() {
  display.init(240, 320);
  display.setRotation(1);
  display.fillScreen(ST77XX_BLACK);

  pinMode(BOTAO1, INPUT_PULLUP);
  pinMode(BOTAO2, INPUT_PULLUP);
  pinMode(BOTAO3, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(ST77XX_WHITE);
  animacao.expressaoBase();
}

void loop() {
  static unsigned long tempoAnterior = 0;
  static int etapa = 0;
  static int angulos[3];
  static int indiceAngulo = 0;

  unsigned long tempoAtual = millis();
  int leitura = analogRead(PINO_POTENCIOMETRO);
  umidade = map(leitura, 0, 1023, 0, 100);

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

  switch (etapa) {
    case 0:
      display.fillScreen(ST77XX_BLACK);
      animacao.expressaoBase();
      if (tempoAtual - tempoAnterior >= 2000) {
        tempoAnterior = tempoAtual;
        etapa = 1;
      }
      break;

    case 1:
      definirExpressao();
      if (tempoAtual - tempoAnterior >= 2000) {
        tempoAnterior = tempoAtual;
        etapa = 2;
      }
      break;

    case 2:
      for (int i = 0; i < 3; i++) {
        angulos[i] = random(0, 360);
      }
      etapa = 3;
      break;

    case 3:
      if (indiceAngulo < 5) {
        if (tempoAtual - tempoAnterior >= 1000) {
          tempoAnterior = tempoAtual;
          animacao.MoverPupilasParaAngulo(angulos[indiceAngulo]);
          indiceAngulo++;
        }
      } else {
        indiceAngulo = 0;
        etapa = 4;
      }
      break;

    case 4:
      animacao.piscar();
      if (tempoAtual - tempoAnterior >= 1000) {
        tempoAnterior = tempoAtual;
        etapa = 0;
      }
      break;
  }
}

void definirExpressao() {
  if (umidade < umidadeMinima) {
    animacao.expressaoTriste();
  } else if (umidade >= umidadeMinima && umidade <= umidadeMaxima) {
    animacao.expressaoFeliz();
  } else {
    animacao.expressaoCansado();
  }
}

void exibirFaixaUmidade() {
  int leitura = analogRead(PINO_POTENCIOMETRO);
  int porcentagem = map(leitura, 0, 1023, 0, 100);

  display.fillScreen(ST77XX_BLACK);
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
}

void exibirModoAtual() {
  display.fillScreen(ST77XX_BLACK);
  display.setCursor(30, 25);
  if (modoAtual == SUCULENTA) {
    display.print("Suculenta");
  } else if (modoAtual == CACTOS) {
    display.print("Cactos");
  } else {
    display.print("Samambaia");
  }
}

void selecionarModo() {
  selecionandoModo = true;
  resetarTempoTela();
  while (millis() - tempoTela < 3000) {
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(30, 25);
    if (modoAtual == SUCULENTA) {
      display.print("Suculenta");
    } else if (modoAtual == CACTOS) {
      display.print("Cactos");
    } else {
      display.print("Samambaia");
    }

    if (digitalRead(BOTAO3) == LOW) {
      alternarModo();
      resetarTempoTela();
      delay(50);
      display.fillScreen(ST77XX_BLACK);
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
    umidadeMinima = 10;
    umidadeMaxima = 20;
  } else if (modoAtual == CACTOS) {
    modoAtual = SAMAMBAIA;
    umidadeMinima = 40;
    umidadeMaxima = 70;
  } else {
    modoAtual = SUCULENTA;
    umidadeMinima = 20;
    umidadeMaxima = 40;
  }
  exibirModoAtual();
}

void confirmarModo() {
  display.fillScreen(ST77XX_BLACK);
  display.setCursor(40, 25);
  display.print("OK");
  delay(1000);
  animacao.expressaoBase();
}

void resetarTempoTela() {
  tempoTela = millis();
}

void aguardarEntrada() {
  while (millis() - tempoTela < 3000) {
    if (digitalRead(BOTAO2) == LOW) {
      exibirModoAtual();
      resetarTempoTela();
      while (millis() - tempoTela < 3000) {
        if (digitalRead(BOTAO1) == LOW) {
          animacao.expressaoBase();
          return;
        }
        if (digitalRead(BOTAO3) == LOW) {
          selecionarModo();
          return;
        }
      }
      animacao.expressaoBase();
      return;
    }
  }
  animacao.expressaoBase();
}
