#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <mysql.h> //libmysql
using namespace std;

//void insertProduct();

//database connection
//int qstate;
//MYSQL* conn;
//MYSQL_ROW row;
//MYSQL_RES* res;



//uncomment to try database
/*
void insertProduct() {
    string product_id = "", product_name = "Cola", price_per_unit = "500.00", current_quantity = "100", reorder_level = "100";
    string order_details_ID = "", order_ID = "", product_ID = "", quantity = "100", sub_total = "100";

    string insertOrderDeatils_query = "insert into orderdetails(order_details_ID, order_ID, product_ID, quantity, sub_total) values('" + order_details_ID + "', '" + order_ID + "', '" + product_ID + "', '" + quantity + "', '" + sub_total + "')";
    const char* p = insertOrderDeatils_query.c_str();
    qstate = mysql_query(conn, p);
    string insertProduct_query = "insert into product(product_id, product_name, price_per_unit, current_quantity, reorder_level) values('" + product_id + "', '" + product_name + "', '" + price_per_unit + "', '" + current_quantity + "', '" + reorder_level + "')";
    const char* q = insertProduct_query.c_str();
    qstate = mysql_query(conn, q);

    mysql_query(conn, "select product_id, product_name, price_per_unit, current_quantity from product");

    cout << left << setw(11) << "product_id" << setw(17) << "product_name" << setw(20) << "price_per_unit" << setw(20) << "current_quantity" << endl;

    res = mysql_store_result(conn);
    while (row = mysql_fetch_row(res))
    {
        cout << setw(11) << row[0] << setw(17) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << row[4] << endl;
    }

    mysql_query(conn, "select order_details_ID, order_ID, product_ID, quantity, sub_total from orderdetails");

    cout << left << setw(11) << "order_details_ID" << setw(17) << "order_ID" << setw(20) << "product_ID" << setw(20) << "quantity" << endl;

    res = mysql_store_result(conn);
    while (row = mysql_fetch_row(res))
    {
        cout << setw(11) << row[0] << setw(17) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << row[4] << endl;
    }

}
*/
/*
int main() {
    system("cls");                  //clear screen
    system("title My Project");
    db_response::ConnectionFunction();

    //test database connection
    //insertProduct();

    return 0;
}
*/
