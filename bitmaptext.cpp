#include "bitmaptext.h"
//#include "logmsg.h"
#include <cstring>

Letter::Letter(int _x, int _y, int _w, int _h)
{
    x= _x /2048.0f;
    y= 1.0f - _y /2048.0f;
    width= _w /2048.0f;
    height= _h /2048.0f;
    y= y - height;
    vertices = new QPointF[4];

}

Letter::~Letter()
{
    delete[] vertices;
}

BitmapText::BitmapText()
{
    letters[L'!'] = new Letter(685, 856, 49, 214);
    letters[L'"'] = new Letter(776, 856, 94, 214);
    letters[L'#'] = new Letter(1582, 642, 95, 214);
    letters[L'$'] = new Letter(1677, 642, 94, 214);
    letters[L'%'] = new Letter(1771, 642, 185, 214);
    letters[L'&'] = new Letter(103, 856, 158, 214);
    letters[L'\''] = new Letter(734, 856, 42, 214);
    letters[L'('] = new Letter(1222, 642, 65, 214);
    letters[L')'] = new Letter(1287, 642, 59, 214);
    letters[L'+'] = new Letter(1113, 642, 109, 214);
    letters[L','] = new Letter(958, 642, 45, 214);
    letters[L'-'] = new Letter(1052, 642, 61, 214);
    letters[L'.'] = new Letter(918, 642, 40, 214);
    letters[L'/'] = new Letter(261, 856, 58, 214);
    letters[L'0'] = new Letter(0, 642, 93, 214);
    letters[L'1'] = new Letter(93, 642, 85, 214);
    letters[L'2'] = new Letter(178, 642, 90, 214);
    letters[L'3'] = new Letter(268, 642, 88, 214);
    letters[L'4'] = new Letter(356, 642, 93, 214);
    letters[L'5'] = new Letter(449, 642, 94, 214);
    letters[L'6'] = new Letter(543, 642, 93, 214);
    letters[L'7'] = new Letter(636, 642, 96, 214);
    letters[L'8'] = new Letter(732, 642, 93, 214);
    letters[L'9'] = new Letter(825, 642, 93, 214);
    letters[L':'] = new Letter(1003, 642, 49, 214);
    letters[L';'] = new Letter(1529, 642, 53, 214);
    letters[L'<'] = new Letter(375, 856, 111, 214);
    letters[L'>'] = new Letter(486, 856, 110, 214);
    letters[L'?'] = new Letter(596, 856, 89, 214);
    letters[L'@'] = new Letter(1346, 642, 183, 214);
    letters[L'A'] = new Letter(0, 0, 142, 214);
    letters[L'B'] = new Letter(142, 0, 126, 214);
    letters[L'C'] = new Letter(268, 0, 134, 214);
    letters[L'D'] = new Letter(402, 0, 136, 214);
    letters[L'E'] = new Letter(538, 0, 124, 214);
    letters[L'F'] = new Letter(662, 0, 116, 214);
    letters[L'G'] = new Letter(778, 0, 153, 214);
    letters[L'H'] = new Letter(931, 0, 153, 214);
    letters[L'I'] = new Letter(1084, 0, 74, 214);
    letters[L'J'] = new Letter(1158, 0, 99, 214);
    letters[L'K'] = new Letter(1257, 0, 158, 214);
    letters[L'L'] = new Letter(1415, 0, 127, 214);
    letters[L'M'] = new Letter(1542, 0, 186, 214);
    letters[L'N'] = new Letter(1728, 0, 141, 214);
    letters[L'O'] = new Letter(1869, 0, 149, 214);
    letters[L'P'] = new Letter(0, 214, 117, 214);
    letters[L'Q'] = new Letter(117, 214, 149, 214);
    letters[L'R'] = new Letter(266, 214, 145, 214);
    letters[L'S'] = new Letter(411, 214, 104, 214);
    letters[L'T'] = new Letter(515, 214, 127, 214);
    letters[L'U'] = new Letter(642, 214, 141, 214);
    letters[L'V'] = new Letter(783, 214, 141, 214);
    letters[L'W'] = new Letter(924, 214, 198, 214);
    letters[L'X'] = new Letter(1122, 214, 144, 214);
    letters[L'Y'] = new Letter(1266, 214, 142, 214);
    letters[L'Z'] = new Letter(1408, 214, 129, 214);
    letters[L'['] = new Letter(870, 856, 60, 214);
    letters[L'\\'] = new Letter(319, 856, 56, 214);
    letters[L']'] = new Letter(930, 856, 47, 214);
    letters[L'^'] = new Letter(0, 856, 103, 214);
    letters[L'a'] = new Letter(1537, 214, 95, 214);
    letters[L'b'] = new Letter(1632, 214, 102, 214);
    letters[L'c'] = new Letter(1734, 214, 84, 214);
    letters[L'd'] = new Letter(1818, 214, 106, 214);
    letters[L'e'] = new Letter(1924, 214, 84, 214);
    letters[L'f'] = new Letter(0, 428, 82, 214);
    letters[L'g'] = new Letter(82, 428, 96, 214);
    letters[L'h'] = new Letter(178, 428, 104, 214);
    letters[L'i'] = new Letter(282, 428, 54, 214);
    letters[L'j'] = new Letter(336, 428, 49, 214);
    letters[L'k'] = new Letter(385, 428, 113, 214);
    letters[L'l'] = new Letter(498, 428, 54, 214);
    letters[L'm'] = new Letter(552, 428, 160, 214);
    letters[L'n'] = new Letter(712, 428, 103, 214);
    letters[L'o'] = new Letter(815, 428, 93, 214);
    letters[L'p'] = new Letter(908, 428, 102, 214);
    letters[L'q'] = new Letter(1010, 428, 106, 214);
    letters[L'r'] = new Letter(1116, 428, 85, 214);
    letters[L's'] = new Letter(1201, 428, 71, 214);
    letters[L't'] = new Letter(1272, 428, 65, 214);
    letters[L'u'] = new Letter(1337, 428, 103, 214);
    letters[L'v'] = new Letter(1440, 428, 98, 214);
    letters[L'w'] = new Letter(1538, 428, 141, 214);
    letters[L'x'] = new Letter(1679, 428, 97, 214);
    letters[L'y'] = new Letter(1776, 428, 98, 214);
    letters[L'z'] = new Letter(1874, 428, 84, 214);
    letters[L'Ё'] = new Letter(1750, 856, 124, 214);
    letters[L'А'] = new Letter(977, 856, 142, 214);
    letters[L'Б'] = new Letter(1119, 856, 125, 214);
    letters[L'В'] = new Letter(1244, 856, 126, 214);
    letters[L'Г'] = new Letter(1370, 856, 121, 214);
    letters[L'Д'] = new Letter(1491, 856, 135, 214);
    letters[L'Е'] = new Letter(1626, 856, 124, 214);
    letters[L'Ж'] = new Letter(0, 1070, 195, 214);
    letters[L'З'] = new Letter(195, 1070, 99, 214);
    letters[L'И'] = new Letter(294, 1070, 153, 214);
    letters[L'Й'] = new Letter(447, 1070, 153, 214);
    letters[L'К'] = new Letter(600, 1070, 142, 214);
    letters[L'Л'] = new Letter(742, 1070, 146, 214);
    letters[L'М'] = new Letter(888, 1070, 186, 214);
    letters[L'Н'] = new Letter(1074, 1070, 153, 214);
    letters[L'О'] = new Letter(1227, 1070, 149, 214);
    letters[L'П'] = new Letter(1376, 1070, 153, 214);
    letters[L'Р'] = new Letter(1529, 1070, 117, 214);
    letters[L'С'] = new Letter(1646, 1070, 134, 214);
    letters[L'Т'] = new Letter(1780, 1070, 127, 214);
    letters[L'У'] = new Letter(0, 1284, 145, 214);
    letters[L'Ф'] = new Letter(145, 1284, 166, 214);
    letters[L'Х'] = new Letter(311, 1284, 144, 214);
    letters[L'Ц'] = new Letter(455, 1284, 151, 214);
    letters[L'Ч'] = new Letter(606, 1284, 144, 214);
    letters[L'Ш'] = new Letter(750, 1284, 210, 214);
    letters[L'Щ'] = new Letter(960, 1284, 210, 214);
    letters[L'Ъ'] = new Letter(1170, 1284, 146, 214);
    letters[L'Ы'] = new Letter(1316, 1284, 193, 214);
    letters[L'Э'] = new Letter(1607, 1284, 129, 214);
    letters[L'Ю'] = new Letter(1736, 1284, 218, 214);
    letters[L'Я'] = new Letter(0, 1498, 141, 214);
    letters[L'а'] = new Letter(141, 1498, 95, 214);
    letters[L'б'] = new Letter(236, 1498, 93, 214);
    letters[L'в'] = new Letter(329, 1498, 101, 214);
    letters[L'г'] = new Letter(430, 1498, 88, 214);
    letters[L'д'] = new Letter(518, 1498, 94, 214);
    letters[L'е'] = new Letter(612, 1498, 84, 214);
    letters[L'ж'] = new Letter(780, 1498, 144, 214);
    letters[L'з'] = new Letter(924, 1498, 75, 214);
    letters[L'и'] = new Letter(999, 1498, 108, 214);
    letters[L'й'] = new Letter(1107, 1498, 108, 214);
    letters[L'к'] = new Letter(1215, 1498, 112, 214);
    letters[L'л'] = new Letter(1327, 1498, 104, 214);
    letters[L'м'] = new Letter(1431, 1498, 129, 214);
    letters[L'н'] = new Letter(1560, 1498, 108, 214);
    letters[L'о'] = new Letter(1668, 1498, 93, 214);
    letters[L'п'] = new Letter(1761, 1498, 108, 214);
    letters[L'р'] = new Letter(1869, 1498, 102, 214);
    letters[L'с'] = new Letter(0, 1712, 84, 214);
    letters[L'т'] = new Letter(84, 1712, 95, 214);
    letters[L'у'] = new Letter(179, 1712, 98, 214);
    letters[L'ф'] = new Letter(277, 1712, 128, 214);
    letters[L'х'] = new Letter(405, 1712, 97, 214);
    letters[L'ц'] = new Letter(502, 1712, 108, 214);
    letters[L'ч'] = new Letter(610, 1712, 106, 214);
    letters[L'ш'] = new Letter(716, 1712, 160, 214);
    letters[L'щ'] = new Letter(876, 1712, 160, 214);
    letters[L'ъ'] = new Letter(1036, 1712, 110, 214);
    letters[L'ы'] = new Letter(1146, 1712, 148, 214);
    letters[L'ь'] = new Letter(1294, 1712, 98, 214);
    letters[L'э'] = new Letter(1392, 1712, 79, 214);
    letters[L'ю'] = new Letter(1471, 1712, 146, 214);
    letters[L'я'] = new Letter(1617, 1712, 101, 214);
    letters[L'ё'] = new Letter(696, 1498, 84, 214);

}

void BitmapText::initGL(QOpenGLShaderProgram *prog)
{
    DrawingObject::initGL(prog,1);
}

void BitmapText::fillData()
{
    VertexData* vertices = new VertexData[nElements()];
    fillVertexData(vertices);
    buffers[0].bind();
    buffers[0].allocate(vertices, nElements() * sizeof(VertexData));
    delete[] vertices;
}

void BitmapText::draw()
{
    if (needsRedraw)
        fillData();
    buffers[0].bind();
    quintptr offset = 0;

    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    offset += sizeof(QVector3D);
    int texcoordLocation = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Offset for color multiplicator
    offset += sizeof(QVector2D);
    int colorMultLocation = m_program->attributeLocation("a_colormult");
    m_program->enableAttributeArray(colorMultLocation);
    m_program->setAttributeBuffer(colorMultLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawArrays(GL_TRIANGLES, 0, nElements());

}

float BitmapText::textWidth(const char *txt)
{
    float w = -cellSize /2; //-2.0;
    for (unsigned int i=0; i< strlen(txt); i++)
    {
        char c = txt[i];
        Letter *l = letters[c];
        if (!l)
            w = w + 1.0;
        else
            w = w + l->width * 2.0;
    }
    return w  / cellSize;
}

void BitmapText::fillVertexData(VertexData *buf)
{
    int nl = text.length();
    QVector4D ymult (1,1,0,1);
    QVector4D gmult (0, 1 , 0, 1);
    QVector4D* cmult = &ymult;

    float w = 1.3/nl;
    wchar_t wtext[100];
    /*int tl = */text.toWCharArray(wtext);
    float left = -1.0f;
    for (int i =0; i<nl; i++)
    {
        if (letters.count(wtext[i]) != 0)
        {
           addLetterData(buf+6*i, wtext[i], left, w, *cmult);
          //left = left + letters[wtext[i]]->width;
           left = left + w;
        }
        else
        {
            left  = left + w;
            cmult = &gmult;
        }
    }
}

void BitmapText::addLetterData(VertexData *buf, wchar_t ch, float left,
                               float w, const QVector4D& colorMult)
{
      Letter *l = letters[ch];
      if (!l) return;
      float dw = l->width ;
      float dh = l->height ;
      buf[0].texCoord = QVector2D(l->X(), l->Y());
      buf[1].texCoord = QVector2D(l->X() +dw, l->Y());
      buf[2].texCoord = QVector2D(l->X() + dw, l->Y() + dh);
      buf[3].texCoord = buf[2].texCoord;
      buf[4].texCoord = QVector2D(l->X(), l->Y()+dh);
      buf[5].texCoord = buf[0].texCoord;

      buf[0].position =  QVector3D(left, -1, 0);
      buf[1].position =  QVector3D(left + w, -1, 0);
      buf[2].position =  QVector3D(left + w, 1, 0);
      buf[3].position =  buf[2].position;
      buf[4].position =  QVector3D(left, 1, 0);
      buf[5].position =  buf[0].position;
      for (int i=0; i< 6; i++)
          buf[i].colormult = colorMult;
}

void BitmapText::setText(const QString &_text)
{
    text = _text;
    needsRedraw = true;
}
