#ifndef ANIMACAOOLHOS_H
#define ANIMACAOOLHOS_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class AnimacaoOlhos {
public:
    AnimacaoOlhos(Adafruit_SSD1306 &display);

    void desenharOlhos();
    void MoverPupilasParaAngulo(int angulo);
    void RestringirPosicaoPupila(int CentroOlhoX, int CentroOlhoY, int &PupilaX, int &PupilaY);
    void piscar();
    void expressaoCansado();
    void expressaoTriste();
    void expressaoBase();
    void expressaoDesconfiado();
    void expressaoFeliz();

private:
    Adafruit_SSD1306 &_display;

    int TamanhoPupila = 10;  // Tamanho das pupilas
    int RaioOcular = 25;  // Raio do globo ocular

    // Posição dos olhos
    int CentroOlhoX1;
    int CentroOlhoY1;
    int CentroOlhoX2;
    int CentroOlhoY2;

    // Posição das pupilas
    int PupilaX1;
    int PupilaY1;
    int PupilaX2;
    int PupilaY2;
};

#endif
