#include "Animal_card_app.h"
#include <QtGui>


/*
 * card size: 85mm x 55mm
 * barcode strip: 10mm wide
 *                17mm from the top
 *                0mm from the right
 *                4.5mm betweenn bases of lines
 *                3mm thick lines
 *                1mm thin lines
 */

static void print_bits(uint32_t bits, unsigned int length, int space = -1) 
{
    for (int i = length-1; i >= 0; i--) {
        if (i == space-1) printf(" ");
        printf("%d", (bits & (1<<i)) ? 1 : 0);
    }
    printf("\n");
}

static unsigned int num_bits(uint32_t bits)
{
    int count = 0;
    for (int i = 0; i < sizeof(bits)*8; i++) {
        if (bits & (1 << i)) count++;
    }
    return count;
}

void Animal_card_app::drawCard(QPainter& painter, 
        uint32_t barcode_val, double scale,
        bool scale_fonts)
{
    painter.save();

    int num_bits = 13;

    double width = scale*card_width;
    double height = scale*card_height;
    double barcode_width = scale*10;
    double barcode_height = scale*53;
    double thick_height = scale*3;
    double thin_height = scale*1.3;
    double offset = scale*17;
    double title_line_space = scale*6;
    double title_x = scale*5;
    double title_y = scale*20;

    double barcode_font_height = 1;
    double title_font_height = 5;
    double subtitle_font_height = 3;
    if (scale_fonts) {
        barcode_font_height *= scale;
        title_font_height *= scale;
        subtitle_font_height *= scale;
    }



    // Barcode
    double increment = barcode_height/(num_bits-1);

    painter.setFont(QFont("Courier", barcode_font_height));

    for (int i = 0; i < num_bits; ++i) {
        bool on = barcode_val & (1 << i);
        double h = on ? thick_height : thin_height;
        double y = i*increment + offset - h;
        double x = width-barcode_width;
        painter.fillRect(QRectF(x,y, barcode_width, h), 
                Qt::SolidPattern);

        // TODO: Add information about the parity and 
        // binary encoding.
        double text_x = width-barcode_width-barcode_width;
        double text_y = y+h;
        if (i == 0) { 
            painter.drawText(QPointF(text_x, text_y), 
                    "Stop bit");
        } else if (i == 1) { 
            painter.drawText(QPointF(text_x, text_y), 
                    "Even Parity");
        } else if (i == 2) { 
            painter.drawText(QPointF(text_x, text_y), 
                    "Odd Parity");
        } else {
            int column = (1 << i) >> 3;
            painter.drawText(QPointF(text_x, text_y), 
                    QString("%1").arg(column));
        }

    }

    painter.setFont(QFont("Arial", title_font_height));
    painter.drawText(QPointF(title_x, title_y), "5up3r");
    title_y += title_line_space;
    painter.drawText(QPointF(title_x, title_y), "4n1m4l5");
    painter.setFont(QFont("Arial", subtitle_font_height));
    title_y += title_line_space;
    painter.drawText(QPointF(title_x, title_y), QString("Card #%1")
                 .arg(barcode_val >> 3));

    // Draw card
    painter.setPen(Qt::black);
    painter.drawRect(QRectF(0,0, width, height));

    painter.restore();
}



Animal_card_app::Animal_card_app(QWidget *parent)
    : QWidget(parent),
    card_width(55),
    card_height(85)
{
    setWindowTitle(tr("Animal card app"));
    resize(800, 1000);

    printFile();
}


void Animal_card_app::paintEvent(QPaintEvent *event)
{
    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawPage(painter, 10);


}

void Animal_card_app::drawPage(QPainter& painter, double scale,
        bool scale_fonts)
{

    uint32_t i = 53;
    unsigned int n = num_bits(i);
    bool even = n & 1;
    bool odd = !even;

    uint32_t barcode_val = 1 | 
            ((odd ? 1 : 0) << 1) | 
            ((even ? 1 : 0) << 2) |
            (i << 3);

    //painter.translate(10, 10);
    //painter.scale(side / 100.0, side / 100.0);
    drawCard(painter, barcode_val, scale, scale_fonts);

}

void Animal_card_app::printFile()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFileName("cards.pdf");
    printer.setPaperSize(QPrinter::A4);
    QPainter painter;
    painter.begin(&printer);

    double dpmm = (printer.resolution() / 25.4);

    QRectF page =  printer.pageRect(QPrinter::Millimeter);

    double scale = dpmm;
    int gap = 3; // mm between cards
    int num_cols = (page.width()+gap) / (card_width + gap);
    int num_rows = (page.height()+gap) / (card_height + gap);
    int cur_row = 0;
    int cur_col = 0;

    printf("Page size = %f x %f : rows=%d : cols=%d\n",
            page.width(),
            page.height(),
            num_rows,
            num_cols);

    for (uint32_t i = 109; i < 118; i++) {

        unsigned int n = num_bits(i);
        bool even = n & 1;
        bool odd = !even;

        uint32_t barcode_val = 1 | 
        ((odd ? 1 : 0) << 1) | 
        ((even ? 1 : 0) << 2) |
        (i << 3);

        painter.save();

        QPointF offset (
                cur_col * (card_width*scale) + cur_col * (scale*gap),
                cur_row * (card_height*scale) + cur_row * (scale*gap));
        printf("%d:%d: Offset = %f, %f\n", 
                cur_row, cur_col,
                offset.x(), offset.y());

        painter.translate(offset);
        drawCard(painter, barcode_val, scale, true);
        painter.restore();

        cur_col = ++cur_col % num_cols;
        if (cur_col == 0) {
            cur_row = ++cur_row % num_rows;
        }

    }

    painter.end();
}

