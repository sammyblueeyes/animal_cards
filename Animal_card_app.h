#ifndef __Animal_card_app_h_SEEN_
#define __Animal_card_app_h_SEEN_

#include <QWidget>
#include <stdint.h>

class Animal_card_app : public QWidget
{
    Q_OBJECT
    public:
        Animal_card_app(QWidget *parent = 0);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        void drawCard(QPainter& painter, uint32_t barcode_val, 
                double scale = 1.0, bool scale_fonts = true);
        void drawPage(QPainter& painter, double scale = 1.0, 
                bool scale_fonts = true);
        void printFile();

    double card_width;
    double card_height;
};



#endif // __Animal_card_app_h_SEEN_
