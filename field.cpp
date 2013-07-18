#include "field.h"
#include <cstring>

Field::Field()
{
  memset(m_Field, 0, sizeof(m_Field));
}

Field::~Field()
{}

void Field::setValue(uint8_t x, uint8_t y, uint8_t val)
{
  if (x > MAX_X || y > MAX_Y)
    return;
  m_Field[x][y] = val;
}

uint8_t Field::getValue(uint8_t x, uint8_t y) const
{
  if (x > MAX_X || y > MAX_Y)
    return -1;
  return m_Field[x][y];
}

void Field::incValue(uint8_t x, uint8_t y)
{
  setValue(x, y, getValue(x, y) + 1);
}

bool Field::isCorrect()
{
  for(int i=0; i<MAX_X; ++i)
    for(int j=0; j<MAX_Y; ++j)
      if (getValue(i, j) >= 5) return false;
  return true;
}

bool Field::isZeros()
{
  for(int i=0; i<MAX_X; ++i)
    for(int j=0; j<MAX_Y; ++j)
      if (getValue(i, j) != 0) return false;
  return true;
}

Field& Field::operator=(Field &field)
{
  // for (int i = 0; i < MAX_X; ++i)
  //   for (int j = 0; j < MAX_Y; ++j)
  //     setValue(i, j, field.getValue(i, j));
  memcpy(m_Field, field.getField(), sizeof(m_Field));
  return *this;
}

bool Field::operator==(Field &field)
{
  for (int i = 0; i < MAX_X; ++i)
    for (int j = 0; j < MAX_Y; ++j)
      if (getValue(i, j) != field.getValue(i, j))
        return false;
  return true;
}

uint8_t* Field::getField()
{
  return &m_Field[0][0];
}
