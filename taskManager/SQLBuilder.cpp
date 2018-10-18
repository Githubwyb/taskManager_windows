#include "SQLBuilder.hpp"
#include "log.h"
#include <string>
#include <ctype.h>

///////////////////////////////////////// SQL CONDITION /////////////////////////////////////////
/**
     * SQLCondition类的初始化，无返回值
     * @param m_strLeftSide 字段名
     * @param m_enOperator  操作符号
     */
SQLCondition::SQLCondition(const std::string &strLeftSide, ConditionOperator enOperator)
    : m_enOperator(enOperator), m_strLeftSide(strLeftSide) {
    Condition condition = {enOperator, strLeftSide, "", ConditionLink::Where};
    m_sqlCondition.push_back(condition);
}

/**
     * SQLCondition类的初始化，无返回值
     * @param m_strLeftSide  字段名
     * @param m_enOperator 操作符号
     * @param m_strRightSide  字段名对应的数值
     */
SQLCondition::SQLCondition(const std::string &strLeftSide,
                           ConditionOperator enOperator,
                           const std::string &strRightSide) : m_enOperator(enOperator),
    m_strLeftSide(strLeftSide),
    m_strRightSide(strRightSide) {
    Condition condition = {enOperator, strLeftSide, strRightSide, ConditionLink::Where};
    m_sqlCondition.push_back(condition);
}

/**
     * SQLCondition类的析构函数，无返回值
     */
SQLCondition::~SQLCondition() {
}

/**
     * 清空函数，无返回值
     */
void SQLCondition::clear() {
    m_enOperator = ConditionOperator::UNKNOWN;
    m_sortedFields.clear();
    m_strLeftSide.clear();
    m_strRightSide.clear();
    m_sqlCondition.clear();
    m_isOrderBy = false;
    m_isLimit = false;
}

/**
     * 条件拼接函数，实现与运算
     * @param m_strLeftSide 字段名
     * @param m_enOperator 操作符号
     * @param m_strRightSide 字段对应的数值
     * @return 模块指针
     */
SQLCondition &
SQLCondition::AND(const std::string &strLeftSide, ConditionOperator enOperator,
                  const std::string &strRightSide) {
    if (strLeftSide.empty() || strRightSide.empty() || enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Condition condition = {enOperator, strLeftSide, strRightSide, ConditionLink::And};
    m_sqlCondition.push_back(condition);

    return *this;
}

/**
     * 条件拼接函数，实现或运算
     * @param m_strLeftSide  字段名
     * @param m_enOperator   操作符号
     * @param m_strRightSide  字段对应的数值
     * @return  模块指针
     */
SQLCondition &SQLCondition::OR(const std::string &strLeftSide, ConditionOperator enOperator,
                               const std::string &strRightSide) {
    if (strLeftSide.empty() || strRightSide.empty() || enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Condition condition = {enOperator, strLeftSide, strRightSide, ConditionLink::Or};
    m_sqlCondition.push_back(condition);

    return *this;
}

/**
     * 排序函数，指定排序方式
     * @param ilSortedFields 第i各排序字段
     * @return  模块指针
     */
SQLCondition &
SQLCondition::ORDER_BY(const std::initializer_list<SortedField> &ilSortedFields) {
    if (ilSortedFields.size() == 0) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_isOrderBy = true;
    m_sortedFields = ilSortedFields;

    return *this;
}

SQLCondition &SQLCondition::AND(const SQLCondition &subCondition) {
    Condition condition(UNKNOWN, "", "", And);
    condition.m_pChild = std::make_shared<SQLCondition>(subCondition);
    m_sqlCondition.push_back(condition);
    return *this;
}

SQLCondition &SQLCondition::OR(const SQLCondition &subCondition) {
    Condition condition(UNKNOWN, "", "", Or);
    condition.m_pChild = std::make_shared<SQLCondition>(subCondition);
    m_sqlCondition.push_back(condition);
    return *this;
}

/**
    * 数值限制函数，限制数值的范围
    * @param iValue   32位的整型数据
    * @return 模块指针
    */
SQLCondition &SQLCondition::LIMIT(const std::uint32_t value) {
    if (value == 0) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_iLimit = value;
    m_isLimit = true;
    return *this;
}

///////////////////////////////////////// SQL BUILDER /////////////////////////////////////////
/**
     * 构造函数
     */
SQLBuilder::SQLBuilder() : m_condition("", ConditionOperator::UNKNOWN, "") {
}

/**
     * 析构函数
     */
SQLBuilder::~SQLBuilder() {
}

/**
     * 清空函数，清空存放表格名、表格字段、表格数值、排序方式、join类、和m_scWhere类的迭代器，无参数，无返回值
     */
void SQLBuilder::clear() {
    m_tables.clear();
    m_fields.clear();
    m_joins.clear();
    m_condition.clear();
    m_values.clear();
    m_isDistinct = false; //DISRINCT函数标志位失效
    m_isWhere = false;    //WHERE函数标志位失效
    m_isFrom = false;     //FROM函数标志位失效
    m_totalAnd = 0;
    m_totalOr = 0;
    m_combingAnd = false;
    m_combingOr = false;

}

/**
     * 指示说明函数，所查询的结果没有重复值
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::DISTINCT() {
    m_isDistinct = true; //

    return *this;
}

/**
     * SELECT查询函数
     * @param Fields  字段列表
     * @return   模块指针
     */
SQLBuilder &SQLBuilder::SELECT(const std::initializer_list<std::string> &fields) {
    if (fields.size() == 0) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_enQueryType = QueryType::Select; //定义操作方式是查询方式
    m_fields = {fields};               //将字段名放在m_vFields 容器中

    return *this;
}

/**
     * FROM函数，指定操作表格
     * @param Tables    表格列表
     * @return   模块指针
     */
SQLBuilder &SQLBuilder::FROM(const std::initializer_list<std::string> &tables) {
    if (tables.size() == 0) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_isFrom = true;
    m_tables = {tables}; //将表格名放在m_vTables 容器中

    return *this;
}

/**
     * FIELDS函数，提供字段
     * @param Fields  字段列表
     * @return  模块指针
     */
SQLBuilder &SQLBuilder::FIELDS(const std::initializer_list<std::string> &fields) {
    if (fields.size() == 0) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_fields = {fields}; //将字段表放在m_vFields容器中

    return *this;
}

/**
 * VALUESA函数，提供字段对应的值
 * @param Values  字段值列表
 * @return  模块指针
 */
SQLBuilder &SQLBuilder::VALUES(const std::initializer_list<std::string> &values) {
    if (values.size() == 0) {
        LOG_WARN("illegal parameter");
        return *this;
    }

    m_values = {}; //对于数字型数据，用to_string()进行数据类型转换
    for (const auto &temp : values) {
        QString temp1 = QString::fromStdString(temp);
        temp1.replace("'", "''");
        m_values.push_back(temp1.toStdString());
    }

    return *this;
}

/**
     * SET函数，在指定字段插入数值
     * @param strField   字段名
     * @param value    字段对应的值
     * @return    模块指针
     */
SQLBuilder &SQLBuilder::SET(const std::string &strField, const std::string &value) {
    if (strField.empty()) {
        LOG_ERROR("parameter illegal");
        LOG_ERROR("field %s", strField.c_str());
        return *this;
    }
    m_fields.push_back(strField); //将字段名压到m_vFields尾端
    QString temp1 = QString::fromStdString(value);
    temp1.replace("'", "''");
    m_values.push_back(temp1.toStdString());    //将value压到m_vValues尾端

    return *this;
}

/**
     * 内连接函数，返回两各表格共有的数据
     * @param m_leftTable  第一个表格名
     * @param m_leftField   第一个字段名
     * @param m_enOperator   操作符号
     * @param m_rightTable   第二个表格名
     * @param m_rightField   第二个字段名
     * @return   模块指针
     */

SQLBuilder &SQLBuilder::INNER_JOIN(const std::string &leftTable,
                                   const std::string &leftField,
                                   ConditionOperator enOperator,
                                   const std::string &rightTable,
                                   const std::string &rightField) {
    if (leftTable.empty() || leftField.empty() || rightTable.empty() || rightField.empty() ||
            enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Join join = {JoinType::InnerJoin, enOperator, leftTable, leftField, rightTable, rightField};
    m_joins.push_back(join);

    return *this;
}

/**
     * 左拼接函数，以左边的表格为基准，保留右表共有的数据
     * @param m_leftTable 第一个表格名
     * @param m_leftField  第一个字段名
     * @param m_enOperator  操作符号
     * @param m_rightTable  第二个表格名
     * @param m_rightField   第二个字段名
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::LEFT_JOIN(const std::string &leftTable,
                                  const std::string &leftField,
                                  ConditionOperator enOperator,
                                  const std::string &rightTable,
                                  const std::string &rightField) {
    if (leftTable.empty() || leftField.empty() || rightTable.empty() || rightField.empty() ||
            enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Join join = {JoinType::LeftJoin, enOperator, leftTable, leftField, rightTable, rightField};
    m_joins.push_back(join);

    return *this;
}

/**
     * 右拼接函数，以右边的表格为基准，保留左边表格共有的部分
     * @param m_leftTable  第一个表格名
     * @param m_leftField  第一个字段名
     * @param m_enOperator  操作符号
     * @param m_rightTable  第二个表格名
     * @param m_rightField   第二个字段名
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::RIGHT_JOIN(const std::string &leftTable,
                                   const std::string &leftField,
                                   ConditionOperator enOperator,
                                   const std::string &rightTable,
                                   const std::string &rightField) {
    if (leftTable.empty() || leftField.empty() || rightTable.empty() || rightField == "" ||
            enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Join join = {JoinType::RightJoin, enOperator, leftTable, leftField, rightTable, rightField};
    m_joins.push_back(join);

    return *this;
}

/**
     * 全拼接函数，保留左右两个表格的内容
     * @param m_leftTable  第一个表格名
     * @param m_leftField  第一个字段名
     * @param m_enOperator  操作符号
     * @param m_rightTable   第二个表格名
     * @param m_rightField   第二个字段名
     * @return  模块指针
     */
SQLBuilder &SQLBuilder::FULL_JOIN(const std::string &leftTable,
                                  const std::string &leftField,
                                  ConditionOperator enOperator,
                                  const std::string &rightTable,
                                  const std::string &rightField) {
    if (leftTable.empty() || leftField.empty() || rightTable.empty() || rightField.empty() ||
            enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    Join join = {JoinType::FullJoin, enOperator, leftTable, leftField, rightTable, rightField};
    m_joins.push_back(join);

    return *this;
}

/**
     * where函数指定，指定操作条件
     * @param m_strLeftSide  字段名
     * @param m_enOperator  操作符号
     * @return 模块指针
     */
SQLCondition &
SQLBuilder::WHERE(const std::string &strLeftSide, ConditionOperator enOperator) {
    if (strLeftSide.empty() || enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return m_condition;
    }

    m_condition = SQLCondition(strLeftSide, enOperator);

    m_isWhere = true;

    return m_condition;
}

/**
       * WHERE函数，指定操作条件
       * @param m_strLeftSide  字段名
       * @param m_enOperator  操作符号
       * @param m_strRightSide  指定字段数值
       * @return  模块指针
         */
SQLCondition &
SQLBuilder::WHERE(const std::string &strLeftSide, ConditionOperator enOperator,
                  const std::string &m_strRightSide) {
    if (strLeftSide.empty() || enOperator == UNKNOWN) {
        LOG_WARN("illegal parameter");

        return m_condition;
    }

    m_condition = SQLCondition(strLeftSide, enOperator, m_strRightSide);

    m_isWhere = true;

    return m_condition;
}

/**
     * WHERE函数，指定操作条件
     * @param m_strLeftSide  字段名
     * @param m_enOperator  操作符号
     * @param m_strRightSide  指定字段数值,列表类型
     * @return  模块指针
     */
SQLCondition &
SQLBuilder::WHERE(const std::string &strLeftSide, ConditionOperator enOperator,
                  const std::initializer_list<std::string> &m_strRightSide) {
    if (strLeftSide.empty() || enOperator == UNKNOWN || m_strRightSide.size() == 0) {
        LOG_WARN("illegal parameter");

        return m_condition;
    }

    std::string s;
    for (auto it = m_strRightSide.begin(); it != m_strRightSide.end(); it++) {
        if (it != m_strRightSide.end()) {
            s = s.append("'");
            s = s.append(*it);
            s = s.append("'");
            {
                s = s.append(", ");
            }
        }
    }
    s.pop_back();
    s.pop_back();
    m_condition = SQLCondition(strLeftSide, enOperator, s);

    m_isWhere = true;

    return m_condition;
}

/**
     * INSERT函数，插入操作函数，指定位置为表格插入数据
     * @param strTable  表格名
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::INSERT(const std::string &strTable) {
    if (strTable.empty()) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_enQueryType = QueryType::Insert;
    m_tables.push_back(strTable);

    return *this;
}

/**
     * 更新操作函数，更新指定位置的数据
     * @param strTable 表格名
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::UPDATE(const std::string &strTable) {
    if (strTable.empty()) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_enQueryType = QueryType::Update;
    m_tables.push_back(strTable);

    return *this;
}

/**
     * 删除函数，删除表格指定内容
     * @param strTable 表格名
     * @return 模块指针
     */
SQLBuilder &SQLBuilder::DELETE(const std::string &strTable) {
    if (strTable.empty()) {
        LOG_WARN("illegal parameter");

        return *this;
    }
    m_enQueryType = QueryType::Delete;
    m_tables.push_back(strTable);

    return *this;
}

/**
 * replace函数,如果表中有该记录则更新，没有则插入新记录
 * @param strTable 表格列表
 * @return 模块指针
 */
SQLBuilder &SQLBuilder::REPLACE(const std::string &strTable) {
    if (strTable.empty()) {
        LOG_WARN("illegal parameter");
        
        return *this;
    }
    m_enQueryType = QueryType::Replace;
    m_tables.push_back(strTable);
    
    return *this;
}

/**
     * getCondition,获得条件拼接语句
     * @return 条件拼接语句
     */
std::string SQLBuilder::getCondition() {
    if (m_condition.m_sqlCondition.empty()) {
        return "";
    }

    std::string sqlCmd = m_condition.getCondition();
    if (sqlCmd.empty()) {
        LOG_ERROR("no condition here");
        return "";
    }

    sqlCmd = "WHERE " + sqlCmd;
    return sqlCmd;
}

/**
     * select语句返回函数，用来返回一条select语句
     * @return  select操作语句
     */
std::string SQLBuilder::getSelect() {
    std::string sql;
    if (m_fields.empty()) {
        return "";
    }
    sql = "SELECT";
    if (m_isDistinct) {
        sql = sql.append(" ");
        sql = sql.append("DISTINCT");
    }
    int size1 = m_fields.size();
    for (int i = 0; i < size1; i++) {
        if (m_fields[i] == "*") {
            sql = sql.append(" * ");
            break;
        } else {
            sql = sql.append(" ");
            sql = sql.append(m_fields[i]);
            sql = sql.append(",");
        }
    }
    sql.pop_back();
    sql = sql.append(" ");
    sql = sql.append("FROM");
    for (std::vector<std::string>::const_iterator it2 = m_tables.begin();
         it2 != m_tables.end(); it2++) {
        sql = sql.append(" ");
        sql = sql.append(*it2);
    }
    int size2 = m_joins.size();
    for (int i = 0; i < size2; i++) {
        switch (m_joins[i].m_enJoinType) {
        case InnerJoin: {
            sql = sql.append(" ");
            sql = sql.append("INNER JOIN");
            sql = sql.append(" ");
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(" ");
            sql = sql.append("ON ");
            sql = sql.append(m_joins[i].m_leftTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_leftField);
            switch (m_joins[i].m_enOperator) {
            case EQ: {
                sql = sql.append(" = ");
            }
                break;
            default:
                break;
            }
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_rightField);
        }
            break;

        case LeftJoin: {
            sql = sql.append(" ");
            sql = sql.append("LEFT JOIN");
            sql = sql.append(" ");
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(" ");
            sql = sql.append("ON ");
            sql = sql.append(m_joins[i].m_leftTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_leftField);
            switch (m_joins[i].m_enOperator) {
            case EQ: {
                sql = sql.append(" = ");
            }
                break;
            default:
                break;
            }
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_rightField);
        }
            break;

        case RightJoin: {
            sql = sql.append(" ");
            sql = sql.append("RIGHT JOIN");
            sql = sql.append(" ");
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(" ");
            sql = sql.append("ON ");
            sql = sql.append(m_joins[i].m_leftTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_leftField);
            switch (m_joins[i].m_enOperator) {
            case EQ: {
                sql = sql.append(" = ");
            }
                break;
            default:
                break;
            }
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_rightField);
        }
            break;
        case FullJoin: {
            sql = sql.append(" ");
            sql = sql.append("FULL JOIN");
            sql = sql.append(" ");
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(" ");
            sql = sql.append("ON ");
            sql = sql.append(m_joins[i].m_leftTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_leftField);
            switch (m_joins[i].m_enOperator) {
            case EQ: {
                sql = sql.append(" = ");
            }
                break;
            default:
                break;
            }
            sql = sql.append(m_joins[i].m_rightTable);
            sql = sql.append(".");
            sql = sql.append(m_joins[i].m_rightField);
        }
            break;
        default:
            break;
        }
    }
    if (m_isWhere) {
        sql = sql.append(" ");
        sql = sql.append(getCondition());
    }
    if (m_condition.m_isOrderBy) {
        sql = sql.append(" ORDER BY");
        sql = sql.append(" ");
        int size3 = m_condition.m_sortedFields.size();
        for (int i = 0; i < size3; i++) {
            sql = sql.append(m_condition.m_sortedFields[i].first);
            switch (m_condition.m_sortedFields[i].second) {
            case ASC: {
                sql = sql.append(" ASC,");
            }
                break;
            case DESC: {
                sql = sql.append(" DESC,");
            }
                break;
            default:
                break;
            }
        }
        sql.pop_back();
        sql.append(" ");
    }

    if (m_condition.m_isLimit) {
        sql = sql.append(" ");
        sql = sql.append("LIMIT ");
        sql = sql.append("'");
        sql = sql.append(std::to_string(m_condition.m_iLimit));
        sql = sql.append("'");
    }
    return sql;
}

/**
     * @function 获取一条insert操作语句
     * @return
     */
std::string SQLBuilder::getInsert() {
    std::string sql;
    if (m_tables.empty()) {
        LOG_ERROR("table isn't exist");
        return "";
    }
    int size1 = m_fields.size();
    int size2 = m_values.size();
    if (size1 == 0) {
        LOG_ERROR("field size error %d", size1);
        return "";
    } else if (size1 != size2) {
        LOG_ERROR("fields %d don't match the value %d", size1, size2);
        return "";
    }
    sql = "INSERT";
    sql = sql.append(" ");
    sql = sql.append("INTO");
    sql = sql.append(" ");
    for (std::vector<std::string>::const_iterator it1 = m_tables.begin();
         it1 != m_tables.end(); it1++) {
        if (it1 != m_tables.begin()) {
            sql = sql.append(", ");
        }
        sql = sql.append(*it1);
    }

    if (size1 == 0) {
    } else {
        sql = sql.append("(");
        for (std::vector<std::string>::const_iterator it2 = m_fields.begin();
             it2 != m_fields.end(); it2++) {
            if (it2 != m_fields.begin()) {
                sql = sql.append(", ");
            }
            sql = sql.append(*it2);
        }
        sql = sql.append(")");
    }
    sql = sql.append(" VALUES");

    for (std::vector<std::string>::const_iterator it3 = m_values.begin();
         it3 != m_values.end(); it3++) {
        if (it3 == m_values.begin()) {
            sql = sql.append("(");
        } else {
            sql = sql.append(", ");
        }
        sql = sql.append("'");
        sql = sql.append(*it3);
        sql = sql.append("'");
    }
    sql = sql.append(")");

    return sql;
}

/**
     * UPDATE操作语句
     * @return 一条UPDATE语句
     */
std::string SQLBuilder::getUpdate() {
    if (m_tables.empty()) {
        return "";
    }
    std::string sql;
    sql = "UPDATE";
    for (std::vector<std::string>::const_iterator it1 = m_tables.begin();
         it1 != m_tables.end(); it1++) {
        if (it1 == m_tables.begin()) {
            sql = sql.append(" ");
        } else {
            sql = sql.append(", ");
        }
        sql = sql.append(*it1);
    }
    int size1 = m_fields.size();
    int size2 = m_values.size();
    if (size1 == size2) {
        sql = sql.append(" SET");
        for (int i = 0; i < size1; i++) {
            sql = sql.append(" ");
            sql = sql.append(m_fields[i]);
            sql = sql.append(" ");
            sql = sql.append("=");
            sql = sql.append(" '");
            sql = sql.append(m_values[i]);
            sql = sql.append("'");
            if (i < size1 - 1) {
                sql = sql.append(",");
            }
        }
    } else {
        LOG_WARN("illegal parameter");
        return "";
    }

    if (m_isWhere) {
        sql = sql.append(" ");
        sql = sql.append(getCondition());
    }

    return sql;
}

/**
     * DELETE操作语句
     * @return 一条DELETE操作语句
     */
std::string SQLBuilder::getDelete() {
    std::string sql;
    if (m_tables.empty()) {
        return "";
    }
    sql = "DELETE";
    {
        sql = sql.append(" ");
        sql = sql.append("FROM");
        for (std::vector<std::string>::const_iterator it2 = m_tables.begin();
             it2 != m_tables.end(); it2++) {
            if (it2 == m_tables.begin()) {
                sql = sql.append(" ");
            } else {
                sql = sql.append(", ");
            }
            sql = sql.append(*it2);
        }
    }
    if (m_isWhere) {
        sql = sql.append(" ");
        sql = sql.append(getCondition());
    }

    return sql;
}

std::string SQLBuilder::getReplace() {
    std::string sql;
    if (m_tables.empty()) {
        LOG_ERROR("table isn't exist");
        return "";
    }
    auto size1 = m_fields.size();
    auto size2 = m_values.size();
    if (size1 == 0) {
        LOG_ERROR("field size error %u", size1);
        return "";
    } else if (size1 != size2) {
        LOG_ERROR("fields %u don't match the value %u", size1, size2);
        return "";
    }
    sql = "REPLACE";
    sql = sql.append(" ");
    sql = sql.append("INTO");
    sql = sql.append(" ");
    for (std::vector<std::string>::const_iterator it1 = m_tables.begin();
         it1 != m_tables.end(); it1++) {
        if (it1 != m_tables.begin()) {
            sql = sql.append(", ");
        }
        sql = sql.append(*it1);
    }
    
    if (size1 == 0) {
    } else {
        sql = sql.append("(");
        for (std::vector<std::string>::const_iterator it2 = m_fields.begin();
             it2 != m_fields.end(); it2++) {
            if (it2 != m_fields.begin()) {
                sql = sql.append(", ");
            }
            sql = sql.append(*it2);
        }
        sql = sql.append(")");
    }
    sql = sql.append(" VALUES");
    
    for (std::vector<std::string>::const_iterator it3 = m_values.begin();
         it3 != m_values.end(); it3++) {
        if (it3 == m_values.begin()) {
            sql = sql.append("(");
        } else {
            sql = sql.append(", ");
        }
        sql = sql.append("'");
        sql = sql.append(*it3);
        sql = sql.append("'");
    }
    sql = sql.append(")");
    
    return sql;
}


/**
 * SQL操作语句
 * @return 一条SQL语句
 */
QString SQLBuilder::getSQL() {
    std::string sql;
    switch (m_enQueryType) {
    case Select: {
        sql = sql.append(getSelect());
    }
        break;

    case Insert: {
        sql = sql.append(getInsert());
    }
        break;
    case Update: {
        sql = sql.append(getUpdate());
    }
        break;
    case Delete: {
        sql = sql.append(getDelete());
    }
        break;

    case Replace: {
        sql.append(getReplace());
    }
        break;

    default: {
        LOG_ERROR("unkown type");
    }
        break;
    }

    return QString::fromStdString(sql);
}

std::string SQLCondition::getCondition() {
    if (m_sqlCondition.empty()) {
        LOG_ERROR("no condition here");
        return "";
    }

    std::string sqlCmd = "";
    for (Condition &condition : m_sqlCondition) {
        switch (condition.m_enLink) {
        case Where:
            break;

        case And: {
            sqlCmd.append("AND ");
        }
            break;

        case Or: {
            sqlCmd.append("OR ");
        }
            break;

        default: {
            LOG_ERROR("unsupport link %d", condition.m_enLink);
            return "";
        }
            break;
        }
        sqlCmd.append(getOneCondition(condition));
    }

    return sqlCmd;
}

std::string SQLCondition::getOneCondition(const SQLCondition::Condition &condition) {
    if (condition.m_enOperator == UNKNOWN) {
        std::string sqlCmd = condition.m_pChild->getCondition();
        if (sqlCmd.empty()) {
            LOG_ERROR("no condition here");
        }

        if (condition.m_pChild->m_sqlCondition.size() > 1) {
            sqlCmd.pop_back();
            sqlCmd = "(" + sqlCmd + ") ";
        }
        return sqlCmd;
    }

    std::string sqlCmd = condition.m_strLeftSide;
    sqlCmd.append(" ");
    switch (condition.m_enOperator) {
    case EQ: {
        sqlCmd.append("=");
        sqlCmd.append(" ");
    }
        break;

    case NOT_EQ: {
        sqlCmd.append("!=");
        sqlCmd.append(" ");
    }
        break;

    case LESS: {
        sqlCmd.append("<");
        sqlCmd.append(" ");
    }
        break;

    case LIKE: {
        sqlCmd.append("LIKE ");
    }
        break;

    case LESS_EQ: {
        sqlCmd.append("<=");
        sqlCmd.append(" ");
    }
        break;

    case MORE: {
        sqlCmd.append(">");
        sqlCmd.append(" ");
    }
        break;

    case MORE_EQ: {
        sqlCmd.append(">=");
        sqlCmd.append(" ");
    }
        break;

    case ConditionOperator::BETWEEN: {
        sqlCmd.append("BETWEEN ");
        sqlCmd.append(condition.m_strRightSide);
        return sqlCmd;
    }
        break;

    case IS_NULL: {
        sqlCmd.append("IS NULL ");
        return sqlCmd;
    }
        break;

    case IS_NOT_NULL: {
        sqlCmd.append("IS NOT NULL ");
        return sqlCmd;
    }
        break;

    case IN: {
        sqlCmd.append("IN");
        sqlCmd.append("(");
        sqlCmd.append(condition.m_strRightSide);
        sqlCmd.append(")");
        return sqlCmd;
    }
        break;

    default:
        break;
    }
    sqlCmd.append("'");
    sqlCmd.append(condition.m_strRightSide);
    sqlCmd.append("' ");

    return sqlCmd;
}
