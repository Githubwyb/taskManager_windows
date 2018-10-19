#ifndef SQLBuilder_hpp
#define SQLBuilder_hpp

#include <string>
#include <vector>
#include <utility>
#include <initializer_list>
#include <memory>
#include <QString>

enum ConditionOperator //列举操作符的类型；列举查询的方式
{
    UNKNOWN,
    EQ,
    NOT_EQ,
    LESS,
    LESS_EQ,
    MORE,
    MORE_EQ,
    IS_NULL,
    IS_NOT_NULL,
    IN,
    LIKE,
    BETWEEN,
    ON,
    AND
};

enum SortDirection //列举查询的方式
{
    ASC,
    DESC
};

enum ConditionLink //列举条件连接方式
{
    Where,
    And,
    Or,
    Unknown,
};

///////////////////////////////////////// SQLCONDITION /////////////////////////////////////////
class SQLCondition {

public:
    /**
             * condition数据结构，用来存放BETWEEN、AND、OR、WHERE的操作条件
             */
    struct Condition {
        ConditionOperator m_enOperator;
        std::string m_strLeftSide;
        std::string m_strRightSide;
        ConditionLink m_enLink = ConditionLink::Where;
        std::shared_ptr<SQLCondition> m_pChild;

        Condition(ConditionOperator m_enOperator = ConditionOperator::UNKNOWN,
                  std::string m_strLeftSide = "", std::string m_strRightSide = "",
                  ConditionLink m_enLink = ConditionLink::Where) : m_enOperator(m_enOperator),
            m_strLeftSide(m_strLeftSide),
            m_strRightSide(
                m_strRightSide),
            m_enLink(m_enLink),
            m_pChild(nullptr) {
        }
    };

    explicit SQLCondition(const std::string &strLeftSide, ConditionOperator enOperator);

    explicit SQLCondition(const std::string &strLeftSide, ConditionOperator enOperator,
                          const std::string &strRightSide);

    ~SQLCondition();

    typedef std::pair<std::string, SortDirection> SortedField;

    /**
             * clear 函数，清楚SQLCondition类的参数，以及m_SqlCondition 容器的数据
             */
    void clear();

    /**
             * and函数，指定条件拼接方式，并把字段名、字段对应的值，操作符号存到vector中
             * @param m_strLeftSide  字段名
             * @param m_enOperator   操作符号
             * @param m_strRightSide  字段对应的值
             * @return  模块指针
             */
    SQLCondition &AND(const std::string &strLeftSide, ConditionOperator enOperator,
                      const std::string &strRightSide);

    /**
             * or函数，指定条件拼接方式，并把字段名、字段对应的值，操作符号存到vector中
             * @param m_strLeftSide  字段名
             * @param m_enOperator   操作符号
             * @param m_strRightSide  字段对应的值
             * @return  模块指针
             */
    SQLCondition &OR(const std::string &strLeftSide, ConditionOperator enOperator,
                     const std::string &strRightSide);

    /**
             * orderBy函数，指定排序的字段名和排序方式，并把这些信息存到指定的vector中
             * @param ilSortedFields 排序pair列表
             * @return 模块指针
             */
    SQLCondition &ORDER_BY(const std::initializer_list<SortedField> &ilSortedFields);

    /**
             * And函数，用来拼接条件语句
             * @param subCondition  SQLCondition类
             * @return 模块指针
             */
    SQLCondition &AND(const SQLCondition &subCondition);

    /**
            * Or函数，用来拼接条件语句
            * @param subCondition  SQLCondition类
            * @return 模块指针
            */
    SQLCondition &OR(const SQLCondition &subCondition);

    /**
             * LIMIT函数，限制数值的范围
             * @param iValue  字段的值
             * @return 模块指针
             */
    SQLCondition &LIMIT(const uint32_t value);

    std::string getCondition();

    static std::string
    getOneCondition(const SQLCondition::Condition &condition);   //获得and拼接条件语句

    std::vector<Condition> m_sqlCondition;   //存放where的条件, 包括and、or和between
    std::vector<SortedField> m_sortedFields; //存放排序的字段和排序方式
    bool m_isOrderBy = false;                 //定义一个布尔类型的变量，赋值为false，OEDREBY函数的标志位
    //std::shared_ptr<SQLCondition> m_ptrSibling;
    //std::shared_ptr<SQLCondition> m_ptrChild;
    ConditionLink m_enLink = ConditionLink::Unknown;
    uint32_t m_iLimit = 0; //限制字段对应值的范围
    bool m_isLimit = false;

private:
    ConditionOperator m_enOperator = ConditionOperator::UNKNOWN; //操作符号枚举变量
    std::string m_strLeftSide;                                     //字段变量
    std::string m_strRightSide;                                     //字段对应的值变量
};

///////////////////////////////////////// SQL BUILDER /////////////////////////////////////////

class SQLBuilder {

public:
    SQLBuilder();

    ~SQLBuilder();

    /**
             * Clear函数，清楚SQLbuilder类的vector容器，和函数的标志位
             */
    void clear();

    /**
             * GetSQL函数，实现各种操作语句的生成
             * @return  一条SQL操作语句
             */
    QString getSQL();

    /**
             * SELECT函数，指定操作方式是查询方式，存取字段名于指定的vector容器
             * @param Fields  字段列表
             * @return   模块指针
             */
    SQLBuilder &SELECT(const std::initializer_list<std::string> &fields);

    /**
             * DISTINCT函数，指明查询结果为唯一，不存在重复值
             * @return  模块指针
             */
    SQLBuilder &DISTINCT();

    /**
             * FROM函数，指定操作表格名
             * @param Tables  表格列表
             * @return  模块指针
             */
    SQLBuilder &FROM(const std::initializer_list<std::string> &tables);

    /**
             * InnerJoin函数，指定表格连接方式为内连接，把表格连接方式、操作符号、表格名和字段名存在指定的vector中
             * @param m_leftTable  表格名
             * @param m_leftField    表格对应的的字段名
             * @param enCondition   操作符号
             * @param m_rightTable    表格名
             * @param m_rightField    表格对应的字段名
             * @return   模块指针
             */
    SQLBuilder &INNER_JOIN(const std::string &leftTable,
                           const std::string &leftField,
                           ConditionOperator enCondition, const std::string &rightTable,
                           const std::string &rightField);

    /**
             * leftJoin函数，指定表格连接方式为左连接，把表格连接方式、操作符号、表格名和字段名存在指定的vector中
             * @param m_leftTable  表格名
             * @param m_leftField    表格对应的的字段名
             * @param enCondition   操作符号
             * @param m_rightTable    表格名
             * @param m_rightField    表格对应的字段名
             * @return   模块指针
             */
    SQLBuilder &LEFT_JOIN(const std::string &leftTable, const std::string &leftField,
                          ConditionOperator enCondition, const std::string &rightTable,
                          const std::string &rightField);

    /**
             * rightJoin函数，指定表格连接方式为右连接，把表格连接方式、操作符号、表格名和字段名存在指定的vector中
             * @param m_leftTable  表格名
             * @param m_leftField    表格对应的的字段名
             * @param enCondition   操作符号
             * @param m_rightTable    表格名
             * @param m_rightField    表格对应的字段名
             * @return   模块指针
             */
    SQLBuilder &RIGHT_JOIN(const std::string &leftTable, const std::string &leftField,
                           ConditionOperator enCondition, const std::string &rightTable,
                           const std::string &rightField);

    /**
             * fullJoin函数，指定表格连接方式为全连接，把表格连接方式、操作符号、表格名和字段名存在指定的vector中
             * @param m_leftTable  表格名
             * @param m_leftField    表格对应的的字段名
             * @param enCondition   操作符号
             * @param m_rightTable    表格名
             * @param m_rightField    表格对应的字段名
             * @return   模块指针
             */
    SQLBuilder &FULL_JOIN(const std::string &leftTable, const std::string &leftField,
                          ConditionOperator enCondition, const std::string &rightTable,
                          const std::string &rightField);

    /**
             * WHERE函数，指定操作条件，将字段名、操作符号、存到指定的vector中
             * @param m_strLeftSide   字段名
             * @param m_enOperator   操作符号
             * @return 模块指针
             */
    SQLCondition &WHERE(const std::string &strLeftSide, ConditionOperator enOperator);

    /**
             * WHERE函数，指定操作条件，将字段名、操作符号、字段的值存到指定的vector中
             * @param m_strLeftSide 字段名
             * @param m_enOperator 操作符号
             * @param m_strRightSide  IN 和 BETWEEN 操作符对应的值
             * @return  模块指针
             */
    SQLCondition &WHERE(const std::string &strLeftSide, ConditionOperator enOperator,
                        const std::initializer_list<std::string> &strRightSide);

    /**
             * WHERE函数，指定操作条件，将字段名、操作符号、字段的值存到指定的vector中
             * @param m_strLeftSide 字段名
             * @param m_enOperator 操作符号IN
             * @param m_strRightSide  字段对应的值
             * @return  模块指针
             */

    SQLCondition &WHERE(const std::string &strLeftSide, ConditionOperator enOperator,
                        const std::string &m_strRightSide);

    /**
             * insert函数，在指定表的指定字段插入值，并把值存入到指定的vector中
             * @param strTable 表格列表
             * @return 模块指针
             */
    SQLBuilder &INSERT(const std::string &strTable);

    /**
             * fields函数，将字段名存到指定的vector中
             * @param Fields  字段列表
             * @return 模块指针
             */
    SQLBuilder &FIELDS(const std::initializer_list<std::string> &fields);

    /**
             * values函数，将字段对应的值存到对应的vector中
             * @param Values 值列表
             * @return  模块指针
             */
    SQLBuilder &VALUES(const std::initializer_list<std::string> &values);

    /**
             * update函数，指定操作类型为更新操作，更新指定表格指定字段的值，并把值存到对应的vector中
             * @param strTable 表格名列表
             * @return 模块指针
             */
    SQLBuilder &UPDATE(const std::string &strTable);

    /**
             * set函数，在指定位置插入数值，并把字段名和字段对应的值，存到对应的vector中
             * @param strField 字段名
             * @param value 表名
             * @return  模块指针
             */
    SQLBuilder &SET(const std::string &strField, const std::string &value);

    /**
             * delete函数，指定操作方式删除操作，并把表格名存到指定的vector中
             * @param strTable  表格名
             * @return  模块指针
             */
    SQLBuilder &DELETE(const std::string &strTable);

    /**
         * replace函数,如果表中有该记录则更新，没有则插入新记录
         * @param strTable 表格列表
         * @return 模块指针
         */
    SQLBuilder &REPLACE(const std::string &strTable);

private:
    std::string getCondition();   //获得条件拼接语句
    std::string getSelect();      // 获得select查询语句
    std::string getInsert();      //获得insert查询句
    std::string getUpdate();      //获取update查询语句
    std::string getDelete();      //获取delete查询语句
    std::string getReplace();     //获取replace语句

private:
    enum QueryType // 列举查询方式
    {
        Select,
        Insert,
        Update,
        Delete,
        Replace
    };
    enum JoinType // 列举表的连接方式
    {
        InnerJoin,
        LeftJoin,
        RightJoin,
        FullJoin,
        Unknown
    };

    /**
             * join数据结构，用来存放需要拼接表的连接方式--InnerJoin、leftJoin、rightJoin、fullJoiny以及操作符号、表格名、字段名
             */
    struct Join {
        JoinType m_enJoinType = JoinType::Unknown;
        ConditionOperator m_enOperator;
        std::string m_leftTable;
        std::string m_leftField;
        std::string m_rightTable;
        std::string m_rightField;

        Join(JoinType m_enJoinType = JoinType::Unknown,
             ConditionOperator m_enOperator = UNKNOWN, std::string m_strTable1 = "",
             std::string m_strField1 = "", std::string m_strTable2 = "",
             std::string m_strField2 = "") : m_enJoinType(m_enJoinType),
            m_enOperator(m_enOperator),
            m_leftTable(m_strTable1), m_leftField(m_strField1),
            m_rightTable(m_strTable2),
            m_rightField(m_strField2) {
        }
    };

    bool m_isDistinct = false;                     //定义一个布尔类型的变量，赋值为false,DISTINCT函数的标志位
    bool m_isWhere = false;                         //定义一个布尔类型的变量，赋值为false，WHERE函数的标志位
    bool m_isFrom = false;                         //定义一个布尔类型的变量，赋值为false，FROM函数的标志位
    bool m_combingAnd = false;
    bool m_combingOr = false;
    int m_totalAnd = 0;
    int m_totalOr = 0;
    QueryType m_enQueryType = QueryType::Select; //定义一个操作类型变量，赋值为查询方式
    std::vector<std::string> m_tables;             //定义一个字符型m_vTables容器，存放表格名
    std::vector<std::string> m_fields;             //定义一个字符型m_vFileds容器，存放字段名
    std::vector<std::string> m_values;             //定义一个字符型m_vValues容器，存放字段对应的值
    std::vector<Join> m_joins;                     //存放表连接方式的表名、字段名、操作符号
    SQLCondition m_condition;                     //定义一个SQLCondition 类的m_Condition实例
};

#endif // SQLBuilder_hpp

