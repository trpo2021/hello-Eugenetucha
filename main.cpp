#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cassert>
 
// контекст для рисования
struct context
{
    context(size_t width, size_t height)
        : m_buf(width * height, ' '), m_width(width), m_height(height)
    {
        assert(width);
        assert(height);
        assert(width <= m_buf.max_size() / height);
    }
 
    void put_char(int x, int y, char ch)
    {
        if ((0 <= x) && (size_t(x) < m_width) &&
            (0 <= y) && (size_t(y) < m_height))
        {
            m_buf[size_t(y) * m_width + size_t(x)] = ch;
        }
    }
 
    // т.к. не используем системное API,
    // результат печати в консоль зависит от корректности
    // параметров ширины @m_width и высоты @m_height области рисования
    std::ostream& flush(std::ostream& target) const
    {
        auto out = std::ostream_iterator<char>(target, "");
        for (auto first = m_buf.cbegin(); first != m_buf.cend(); target << std::endl)
        {
            auto last = first;
            std::advance(last, m_width);
            std::copy(first, last, out);
            first = last;
        }
        return target;
    }
 
private:
    std::vector<char> m_buf;
    size_t m_width;
    size_t m_height;
};
 
// алгоритм из википедии, все претензии туда
// (ну и что бы в консоли получилась окружность, нужно рисовать эллипс, но это сам делай, если надо)
context& draw_circle(context& target, char ch, int center_x, int center_y, int radius)
{
    int x = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;
    while (y >= 0)
    {
        target.put_char(center_x + x, center_y + y, ch);
        target.put_char(center_x + x, center_y - y, ch);
        target.put_char(center_x - x, center_y + y, ch);
        target.put_char(center_x - x, center_y - y, ch);
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0))
            delta += 2 * ++x + 1;
        else if ((delta > 0) && (error > 0))
            delta -= 2 * --y + 1;
        else
            delta += 2 * (++x - y--);
    }
    return target;
}
 
int main()
{
    context ctx(80, 20);
    draw_circle(ctx, '0', 40, 10, 5);
    draw_circle(ctx, '1', 40, 10, 10);
    draw_circle(ctx, '2', 40, 10, 15).flush(std::cout);
    std::cin.get();
}
