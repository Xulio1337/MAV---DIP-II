#include "AnimacaoOlhos.h"

AnimacaoOlhos::AnimacaoOlhos(Adafruit_ST7789 &display) : _display(display) {
    CentroOlhoX1 = _display.width() / 1.3;
    CentroOlhoY1 = _display.height() / 2;
    CentroOlhoX2 = _display.width() / 4;
    CentroOlhoY2 = _display.height() / 2;

    PupilaX1 = CentroOlhoX1;
    PupilaY1 = CentroOlhoY1;
    PupilaX2 = CentroOlhoX2;
    PupilaY2 = CentroOlhoY2;
}

void AnimacaoOlhos::desenharOlhos() {
    _display.clearDisplay();
    _display.fillCircle(CentroOlhoX1, CentroOlhoY1, RaioOcular, WHITE);
    _display.fillCircle(CentroOlhoX2, CentroOlhoY2, RaioOcular, WHITE);
    _display.fillCircle(PupilaX1, PupilaY1, TamanhoPupila, BLACK);
    _display.fillCircle(PupilaX2, PupilaY2, TamanhoPupila, BLACK);
    _display.display();
}

void AnimacaoOlhos::MoverPupilasParaAngulo(int angulo) {
    int distancia = random(0, RaioOcular - TamanhoPupila);

    int novaPupilaX1 = CentroOlhoX1 + distancia * cos(radians(angulo));
    int novaPupilaY1 = CentroOlhoY1 + distancia * sin(radians(angulo));
    int novaPupilaX2 = CentroOlhoX2 + distancia * cos(radians(angulo));
    int novaPupilaY2 = CentroOlhoY2 + distancia * sin(radians(angulo));

    int deslocamentoX1 = (novaPupilaX1 - PupilaX1) / 3;
    int deslocamentoY1 = (novaPupilaY1 - PupilaY1) / 3;
    int deslocamentoX2 = (novaPupilaX2 - PupilaX2) / 3;
    int deslocamentoY2 = (novaPupilaY2 - PupilaY2) / 3;

    CentroOlhoX1 += deslocamentoX1;
    CentroOlhoY1 += deslocamentoY1;
    CentroOlhoX2 += deslocamentoX2;
    CentroOlhoY2 += deslocamentoY2;

    PupilaX1 = novaPupilaX1;
    PupilaY1 = novaPupilaY1;
    PupilaX2 = novaPupilaX2;
    PupilaY2 = novaPupilaY2;

    RestringirPosicaoPupila(CentroOlhoX1, CentroOlhoY1, PupilaX1, PupilaY1);
    RestringirPosicaoPupila(CentroOlhoX2, CentroOlhoY2, PupilaX2, PupilaY2);

    _display.clearDisplay();
    _display.fillCircle(CentroOlhoX1, CentroOlhoY1, RaioOcular, WHITE);
    _display.fillCircle(CentroOlhoX2, CentroOlhoY2, RaioOcular, WHITE);
    _display.fillCircle(PupilaX1, PupilaY1, TamanhoPupila, BLACK);
    _display.fillCircle(PupilaX2, PupilaY2, TamanhoPupila, BLACK);
    _display.display();
}

void AnimacaoOlhos::RestringirPosicaoPupila(int CentroOlhoX, int CentroOlhoY, int &PupilaX, int &PupilaY) {
    if (PupilaX < CentroOlhoX - RaioOcular + TamanhoPupila) PupilaX = CentroOlhoX - RaioOcular + TamanhoPupila;
    if (PupilaX > CentroOlhoX + RaioOcular - TamanhoPupila) PupilaX = CentroOlhoX + RaioOcular - TamanhoPupila;
    if (PupilaY < CentroOlhoY - RaioOcular + TamanhoPupila) PupilaY = CentroOlhoY - RaioOcular + TamanhoPupila;
    if (PupilaY > CentroOlhoY + RaioOcular - TamanhoPupila) PupilaY = CentroOlhoY + RaioOcular - TamanhoPupila;
}

void AnimacaoOlhos::piscar() {
    for (int i = 1; i <= 32; i++) {
        if (i >= 16 && i <= 48) continue;
        _display.drawLine(0, i - 1, _display.width() - 1, i - 1, BLACK);
        _display.drawLine(0, _display.height() - i, _display.width() - 1, _display.height() - i, BLACK);
    }
    _display.display();
    delay(5);

    for (int i = 1; i <= 32; i++) {
        if (i >= 25 && i <= 39) continue;
        _display.drawLine(0, i - 1, _display.width() - 1, i - 1, BLACK);
        _display.drawLine(0, _display.height() - i, _display.width() - 1, _display.height() - i, BLACK);
    }
    _display.display();
    delay(5);

    for (int i = 1; i <= 32; i++) {
        if (i >= 32 && i <= 32) continue;
        _display.drawLine(0, i - 1, _display.width() - 1, i - 1, BLACK);
        _display.drawLine(0, _display.height() - i, _display.width() - 1, _display.height() - i, BLACK);
    }
    _display.display();
    delay(5);

    _display.clearDisplay();
    _display.display();
    delay(5);
}

void AnimacaoOlhos::expressaoCansado() {
    for (int i = 0; i <= 26; i++) {
        _display.drawLine(0, i, _display.width() - 1, i, BLACK);
    }
    _display.display();
}

void AnimacaoOlhos::expressaoTriste() {
    int x1 = 0, y1 = 0;
    int x2 = _display.width() / 2, y2 = 0;
    int x3 = 0, y3 = _display.height() / 2;

    _display.fillTriangle(x1, y1, x2, y2, x3, y3, BLACK);

    int x4 = _display.width() - 1, y4 = 0;
    int x5 = _display.width() / 2, y5 = 0;
    int x6 = _display.width() - 1, y6 = _display.height() / 2;

    int x_centro = _display.width() / 2;
    int y_centro = _display.height() - 5;
    int raio = 8;
    int espessura = 2;

    for (int i = 200; i <= 340; i++) {
        for (int j = -espessura; j <= espessura; j++) {
            int x = x_centro + raio * cos(i * PI / 180);
            int y = y_centro + raio * sin(i * PI / 180);
            _display.drawPixel(x, y, WHITE);
        }
    }

    _display.fillTriangle(x4, y4, x5, y5, x6, y6, BLACK);
    _display.display();
}

void AnimacaoOlhos::expressaoBase() {
    CentroOlhoX1 = _display.width() / 1.3;
    CentroOlhoY1 = _display.height() / 2;
    CentroOlhoX2 = _display.width() / 4;
    CentroOlhoY2 = _display.height() / 2;

    PupilaX1 = CentroOlhoX1;
    PupilaY1 = CentroOlhoY1;
    PupilaX2 = CentroOlhoX2;
    PupilaY2 = CentroOlhoY2;

    _display.clearDisplay();
    _display.fillCircle(CentroOlhoX1, CentroOlhoY1, RaioOcular, WHITE);
    _display.fillCircle(CentroOlhoX2, CentroOlhoY2, RaioOcular, WHITE);
    _display.fillCircle(PupilaX1, PupilaY1, TamanhoPupila, BLACK);
    _display.fillCircle(PupilaX2, PupilaY2, TamanhoPupila, BLACK);
    _display.display();
}

void AnimacaoOlhos::expressaoDesconfiado() {
    for (int i = 0; i <= 26; i++) {
        _display.drawLine(0, i, _display.width() - 1, i, BLACK);
    }
    int x1 = 0, y1 = _display.height() - 1;
    int x2 = _display.width() - 1, y2 = _display.height() - 1;
    int x3 = _display.width() / 2, y3 = _display.height() / 2;

    _display.fillTriangle(x1, y1, x2, y2, x3, y3, BLACK);
    _display.display();
}

void AnimacaoOlhos::expressaoFeliz() {
    int x1 = _display.width() / 4;
    int y1 = _display.height() - 1;
    int raio = 45;

    _display.fillCircle(x1, y1, raio, BLACK);

    int x2 = _display.width() / 1.3;
    int y2 = _display.height() - 1;

    _display.fillCircle(x2, y2, raio, BLACK);

    int x_centro1 = _display.width() / 2;
    int y_centro1 = _display.height() - 20;
    int raio2 = 10;
    int espessura = 1;

    for (int i = 20; i <= 160; i++) {
        for (int j = -espessura; j <= espessura; j++) {
            int x = x_centro1 + (raio2 + j) * cos(i * PI / 180);
            int y = y_centro1 + (raio2 + j) * sin(i * PI / 180);
            _display.drawPixel(x, y, WHITE);
        }
    }
    _display.display();
}
