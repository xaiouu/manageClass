#include "CourseTable.h"
#include <FL/fl_draw.H>

/**
 * @brief 表格的事件回调函数
 * @details 处理表格的点击事件，实现行选择功能
 * @param w 触发事件的控件指针
 * @param userdata 用户数据指针（未使用）
 */
static void static_callback(Fl_Widget *w, void *userdata)
{
    CourseTable *table = (CourseTable *)w;

    if (!table)
        return;

    // 获取点击的行和列
    int R = table->callback_row();
    table->set_callback_row(R); // 更新选中行
    int C = table->callback_col();

    // 如果点击位置有效，选中整行
    if (R != -1 && C != -1)
    {
        table->select_row_custom(R);
    }
}

/**
 * @brief 表格构造函数
 * @details 初始化表格属性和事件处理
 */
CourseTable::CourseTable(int x, int y, int w, int h, const char *l)
    : Fl_Table(x, y, w, h, l), callback_row_(-1)
{
    // 设置表格属性
    selection_color(FL_YELLOW); // 选中行的颜色
    when(FL_WHEN_RELEASE);      // 鼠标释放时触发事件

    // 添加事件处理
    Fl_Table::callback(static_callback);
}

/**
 * @brief 绘制表格单元格
 * @details 根据不同的上下文绘制单元格、表头等
 */
void CourseTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H)
{
    switch (context)
    {
    case CONTEXT_CELL: // 绘制数据单元格
    {
        fl_push_clip(X, Y, W, H); // 设置裁剪区域
        fl_color(FL_WHITE);       // 设置背景色
        fl_rectf(X, Y, W, H);     // 填充背景
        fl_color(FL_GRAY0);       // 设置边框色
        fl_rect(X, Y, W, H);      // 绘制边框

        // 如果单元格有数据，绘制文本
        if (R < (int)tableData.size() && C < (int)tableData[R].size())
        {
            fl_draw(tableData[R][C].c_str(), X + 2, Y, W - 4, H, FL_ALIGN_LEFT | FL_ALIGN_WRAP);
        }
        fl_pop_clip();
        break;
    }
    case CONTEXT_COL_HEADER: // 绘制列表头
        DrawHeader("", X, Y, W, H);
        break;
    case CONTEXT_ROW_HEADER: // 绘制行表头
        DrawHeader("", X, Y, W, H);
        break;
    }
}

/**
 * @brief 绘制表头
 * @details 使用特殊的样式绘制表头单元格
 */
void CourseTable::DrawHeader(const char *s, int X, int Y, int W, int H)
{
    fl_push_clip(X, Y, W, H);
    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

/**
 * @brief 添加一行数据到表格
 * @details 更新数据并重绘表格
 */
void CourseTable::addRow(const std::vector<std::string> &rowData)
{
    tableData.push_back(rowData);
    rows(tableData.size()); // 更新表格行数
    redraw();               // 重绘表格
}

/**
 * @brief 清空表格数据
 */
void CourseTable::clearData()
{
    tableData.clear();
    rows(0);  // 设置行数为0
    redraw(); // 重绘表格
}

/**
 * @brief 设置单元格的值
 * @details 检查索引有效性并更新数据
 */
void CourseTable::set_cell_value(int row, int col, const char *value)
{
    if (row >= 0 && row < (int)tableData.size() &&
        col >= 0 && col < (int)tableData[row].size())
    {
        tableData[row][col] = value;
        redraw(); // 重绘表格
    }
}

/**
 * @brief 获取单元格的值
 * @return 单元格内容，如果索引无效则返回空字符串
 */
std::string CourseTable::get_cell_value(int row, int col)
{
    if (row >= 0 && row < (int)tableData.size() &&
        col >= 0 && col < (int)tableData[row].size())
    {
        return tableData[row][col];
    }
    return "";
}

/**
 * @brief 设置当前选中的行
 */
void CourseTable::set_callback_row(int row)
{
    callback_row_ = row;
    redraw(); // 重绘表格以更新选中状态
}

/**
 * @brief 选中指定行
 * @details 清除之前的选择并高亮显示新选中的行
 */
void CourseTable::select_row_custom(int row)
{
    // 清除之前的选择
    for (int i = 0; i < rows(); i++)
    {
        for (int j = 0; j < cols(); j++)
        {
            set_selection(0, 0, 0, 0); // 清除选择
        }
    }

    // 选择新行
    if (row >= 0 && row < rows())
    {
        set_selection(row, 0, row, cols() - 1); // 选择整行
    }

    redraw(); // 重绘表格以显示选择效果
}