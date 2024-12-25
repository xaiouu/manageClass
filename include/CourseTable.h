#ifndef COURSE_TABLE_H
#define COURSE_TABLE_H

#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <vector>
#include <string>

/**
 * @brief 自定义表格控件类，用于显示课程、教室和课表信息     // 不是课表
 * @inherit Fl_Table FLTK的表格基类
 */
class CourseTable : public Fl_Table
{
private:
    // 存储表格数据的二维vector，每行是一个字符串数组
    std::vector<std::vector<std::string>> tableData;

    // 绘制表头的辅助函数
    void DrawHeader(const char *s, int X, int Y, int W, int H);

    // 记录当前选中的行
    int callback_row_;

protected:
    /**
     * @brief 绘制表格单元格的虚函数
     * @param context 绘制上下文（单元格、表头等）
     * @param R 行号
     * @param C 列号
     * @param X,Y 绘制位置
     * @param W,H 单元格大小
     */
    void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
    enum ResizeFlag
    {
        RESIZE_NONE,
        RESIZE_COL,
        RESIZE_ROW
    };

public:
    /**
     * @brief 构造函数
     * @param x,y 表格位置
     * @param w,h 表格大小
     * @param l 表格标签
     */
    CourseTable(int x, int y, int w, int h, const char *l = 0);

    /**
     * @brief 添加一行数据到表格
     * @param rowData 一行的字符串数组
     */
    void addRow(const std::vector<std::string> &rowData);

    /**
     * @brief 清空表格所有数据
     */
    void clearData();

    /**
     * @brief 设置指定单元格的值
     * @param row,col 单元格位置
     * @param value 要设置的值
     */
    void set_cell_value(int row, int col, const char *value);

    /**
     * @brief 获取指定单元格的值
     * @return 单元格内容
     */
    std::string get_cell_value(int row, int col);

    /**
     * @brief 获取当前选中的行
     * @return 选中的行号，-1表示未选中
     */
    int callback_row() const { return callback_row_; }

    /**
     * @brief 设置当前选中的行
     * @param row 要选中的行号
     */
    void set_callback_row(int row);

    /**
     * @brief 选中指定行（高亮显示）
     * @param row 要选中的行号
     */
    void select_row_custom(int row);
};

#endif