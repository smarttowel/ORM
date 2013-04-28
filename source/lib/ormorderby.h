#ifndef ORMORDERBY_H
#define ORMORDERBY_H

#include <QString>

/*!
   \class ORMOrderBy
   \brief The ORMOrderBy class use for generate \a ORDER \a BY expressions.

   You must create object and pass them \a fieldName and \a type of sort.
 */

class ORMOrderBy
{
public:
    /*!
    \enum ORMOrderBy::OrderType
    \brief Enumeration of type of sort.
    \var ORMOrderBy::OrderType ORMOrderBy::Ascending
    Equivalent for SQL \a ASC.
    \var ORMOrderBy::OrderType ORMOrderBy::Descending
    Equivalent for SQL \a DESC
    */
    enum OrderType
    {
        Ascending,
        Descending
    };
    /*!
      Constructs empty ORMOrderBy object.
     */
    ORMOrderBy();
    /*!
       Constructs ORMOrderBy object with the given \a fieldName and \a type.
     */
    ORMOrderBy(QString fieldName, OrderType type);
    /*!
       Returns current \a ORDER \a BY expression.
     */
    QString getOrderString() const;
    /*!
       Сombines two \a ORDER \a BY into a single expression.
     */
    ORMOrderBy& operator&& (ORMOrderBy b);
    /*!
       Assigns the b object to this object.
     */
    void operator= (ORMOrderBy b);

private:
    QString m_orderString;
};

#endif // ORMORDERBY_H
