#ifndef __Animal_card_app_h_SEEN_
#define __Animal_card_app_h_SEEN_

#include <stdint.h>

class QPainter;

class Animal_card_app
{
    public:
        Animal_card_app();
        void printFile(uint32_t start = 1, uint32_t end = 1023, 
                const char* filename = 0);

    private:
        void drawCard(QPainter& painter, uint32_t barcode_val, 
                double scale = 1.0, bool scale_fonts = true);
        double card_width;
        double card_height;

        static const unsigned int NUM_BITS = 13;
        static const unsigned int NUM_ID_BITS = 10;
        static const unsigned int MIN_ID = 1;
};



#endif // __Animal_card_app_h_SEEN_
