#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <mysql.h>	 //libmysql
using namespace std;

int qstate;
MYSQL* conn;	//database connection
MYSQL_ROW row;	//to store one row
MYSQL_RES* res; //return result from query

#pragma warning(disable : 4996)
time_t today = time(0);
tm* ltm = localtime(&today);
int Tday = ltm->tm_mday;
int Tmonth = 1 + ltm->tm_mon;
int Tyear = 1900 + ltm->tm_year;
string T_day = (Tday < 10) ? "0" + to_string(Tday) : to_string(Tday);
string T_month = (Tmonth < 10) ? "0" + to_string(Tmonth) : to_string(Tmonth);
string T_year = to_string(Tyear);
string today_date = T_year + "-" + T_month + "-" + T_day;

string user_account_ID;
//payment_method: 1 - Cash, 2-Cheque, 3 - Bank-in, 4 - Credit
//payment_status: 1 - Fully paid, 2 - Partially paid, 3 - Pending, 4 - Overdue
//delivery_status: 1 - Not prepared yet, 2 - Prepared, 3 - Delivered

//User Management
void WelcomePage();
void Login();
void SignUp();
//Customer's Part
void CustomerMainMenu();
void PlaceOrder();
void Invoice(string, string);
//Staff's Part
void StaffMainMenu();
void OrderList();
void DeliveryNote(string);
void UpdateOrder();
void DefaulterList();
void AddStock();
void StockReport();

class db_response
{
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (conn)
			cout << "Database Connected" << endl;
		else
			cout << "Failed To Connect!" << endl;

		conn = mysql_real_connect(conn, "localhost", "root", "", "testing", 3306, NULL, 0);
		if (conn)
		{
			cout << "Database Connected To MySql" << endl;
			cout << endl << "Press enter to continue..." << endl;
		}
		else
			cout << "Failed To Connect!" << endl;

		_getch();
		system("cls");
	}
};

void WelcomePage() {
	char selection;
	do {
		system("cls");
		cout << "\nBeverage Warehouse Management System" << endl << endl;
		cout << "1 - Log In\n2 - Sign Up\n0 - Exit\n\n";
		cout << "Enter your selection: ";
		cin >> selection;

		if (selection == '1') {
			Login();
		}
		else if (selection == '2') {
			SignUp();
		}
		else if (selection == '0') {
			cout << "\nExiting...\nProgram Terminated.\n";
			break;
		}
		else
			cout << "Invalid selection.Please try again.";
	} while (selection != '0' && selection != '1' && selection != '2');
}

void Login()
{
	system("cls");
	string userID, password;
	cout << endl << setw(13) << "LOG IN" << endl << endl;
	cout << "User ID: ";
	cin >> userID;
	cout << "Password: ";
	char pw;
	while ((pw = _getch()) != 13)
	{
		password += pw;
		cout << '*';
	}

	string checkIDPW_query = "SELECT * FROM useraccount WHERE user_account_ID = '" + userID + "' AND password = '" + password + "'";
	const char* login = checkIDPW_query.c_str();
	qstate = mysql_query(conn, login);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			while (row = mysql_fetch_row(res))
			{
				string access_type, username;
				user_account_ID = row[0];
				username = row[1];
				access_type = row[3];

				cout << setw(27) << "Hi, " << username << "!" << endl;
				cout << "Please select an option by entering the number labelled." << endl;

				if (access_type == "A")
					cout << "Administrator";
				else if (access_type == "S")
					StaffMainMenu();
				else
					CustomerMainMenu();
			}
		}
		else
		{
			char b;
			cout << "\n\nWrong User ID or Password. \nDo you want to try again? (Y/N): ";
			do {
				cin >> b;
				if (b == 'y' || b == 'Y')
					Login();
				else if (b == 'n' || b == 'N')
					WelcomePage();
				else
				{
					cout << "Invalid input." << endl;
					cout << "Please enter Y for yes or N for no: ";
				}
			} while (b != 'y' && b != 'Y' && b != 'n' && b != 'N');
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void SignUp()
{
	system("cls");

	string cust_name, cust_tel, cust_add1, cust_add2, cust_area, cust_postcode, cust_state, username, password, password2, userID;
	string access_type = "C";
	cout << setw(20) << "SIGN UP" << endl << endl;
	cout << "Please fill the details to register an account." << endl;
	cout << "Name: ";
	cin.ignore(1, '\n');
	getline(cin, cust_name);
	cout << "Contact number: ";
	getline(cin, cust_tel);
	cout << "Address: ";
	getline(cin, cust_add1);
	cout << "Address 2: ";
	getline(cin, cust_add2);
	cout << "Area: ";
	getline(cin, cust_area);
	cout << "Postcode: ";
	getline(cin, cust_postcode);
	cout << "State: ";
	getline(cin, cust_state);
	cout << "Username: ";
	getline(cin, username);
	do {
		cout << "Password: ";
		getline(cin, password);
		cout << "Re-enter password: ";
		getline(cin, password2);
		if (password != password2)
			cout << "Please enter the same password to verify. Try again.\n";
	} while (password != password2);

	if (cust_name == "" || cust_tel == "" || cust_add1 == "" || cust_area == "" || cust_postcode == "" || cust_state == "" || username == "" || password == "" || password2 == "")
	{
		cout << "Please enter ALL of the details. Press enter to try again...";
		_getch();
		SignUp();
	}
	else
	{
		string insertNewUserAcc_query = "INSERT INTO useraccount (username, password, access_type) values ('" + username + "', '" + password + "', '" + access_type + "')";
		const char* newUA = insertNewUserAcc_query.c_str();
		qstate = mysql_query(conn, newUA);

		if (qstate)
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		else
		{
			string checkUserID_query = "SELECT user_account_ID FROM useraccount WHERE username = '" + username + "' AND password = '" + password + "'";
			const char* checkUserID = checkUserID_query.c_str();
			qstate = mysql_query(conn, checkUserID);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					userID = row[0];
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			string insertNewCustomer_query = "INSERT INTO customer (cust_name, cust_tel, cust_address1, cust_address2, cust_area, cust_postcode, cust_state, user_account_ID) VALUES ('" + cust_name + "', '" + cust_tel + "', '" + cust_add1 + "', '" + cust_add2 + "', '" + cust_area + "', '" + cust_postcode + "', '" + cust_state + "', '" + userID + "')";
			const char* newCust = insertNewCustomer_query.c_str();
			qstate = mysql_query(conn, newCust);

			if (!qstate)
			{
				cout << endl << "Successfully registered!\n";
				cout << "Your user ID is " << userID << ".\n";
				cout << "Please log in with your userID and password.\n";
				cout << "\nPress enter to continue...";
				_getch();
				WelcomePage();
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
	}
}

void CustomerMainMenu()
{
	system("cls");
	char select;
	cout << endl << right << setw(13) << "MAIN MENU" << endl << endl;
	cout << "1 - Place order\n2 - View Invoice\n0 - Log out\n\n";
	cout << "Enter your selection: ";
	cin >> select;

	if (select == '1')
		PlaceOrder();
	else if (select == '2') {
		string custid, orderid, order_id;

		system("cls");
		cout << right << setw(12) << "INVOICE" << endl << endl;

		string checkCustID_query = "SELECT cust_ID FROM customer WHERE user_account_ID = '" + user_account_ID + "'";
		const char* checkCustID = checkCustID_query.c_str();
		qstate = mysql_query(conn, checkCustID);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
				custid = row[0];
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "Orders: " << endl;
		string checkOrderID_query = "SELECT * FROM orders WHERE cust_ID = '" + custid + "'";
		const char* checkOrderID = checkOrderID_query.c_str();
		qstate = mysql_query(conn, checkOrderID);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
				cout << row[0] << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\nEnter order ID: ";
		cin.ignore(1, '\n');
		getline(cin, orderid);

		string checkOrderid_query = "SELECT * FROM orders WHERE cust_ID = '" + custid + "'";
		const char* checkOrderid = checkOrderid_query.c_str();
		qstate = mysql_query(conn, checkOrderid);
		if (!qstate)
		{
			bool orderFound = false;
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				if (orderid == row[0]) {
					order_id = row[0];
					orderFound = true;
					break;
				}
			}
			if (orderFound == false) {
				cout << "This order is not exist. Press enter to continue..." << endl;
				_getch();
				system("cls");
				CustomerMainMenu();
			}
			else
				Invoice(order_id, custid);
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	else if (select == '0') {
		cout << "\nLogging out...\n";
		WelcomePage();
	}
	else {
		cout << "Invalid selection.Please try again.";
		CustomerMainMenu();
	}
}

void PlaceOrder()
{
	system("cls");
	cout << right << setw(36) << "PLACE ORDER" << endl << endl;
	string orderID;

	qstate = mysql_query(conn, "SELECT product_ID, product_name, price_per_unit, current_quantity FROM product");
	if (!qstate)
	{
		cout << setw(37) << "PRODUCT LIST" << endl;
		cout << left << setw(15) << "Product ID" << setw(20) << "Product Name" << setw(15) << "Price/unit" << setw(10) << "Stock Left" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << left << setw(15) << row[0] << setw(20) << row[1] << setw(15) << row[2] << setw(10) << row[3] << endl;

		cout << "\n5% discount for orders over RM10000\n\n";
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	string custID;
	char c;
	cout << "Do you want to order a product? (Y/N): ";
	do {
		cin >> c;
		if (c == 'y' || c == 'Y')
		{
			string checkCustID_query = "SELECT cust_ID FROM customer WHERE user_account_ID = '" + user_account_ID + "'";
			const char* checkCustID = checkCustID_query.c_str();
			qstate = mysql_query(conn, checkCustID);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					custID = row[0];
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			string delivery_status = "1";	// (1 - not prepared yet)
			string insertNewOrder_query = "INSERT INTO orders (order_date, delivery_status, cust_ID) VALUES ('" + today_date + "', '" + delivery_status + "', '" + custID + "')";
			const char* newOrder = insertNewOrder_query.c_str();
			qstate = mysql_query(conn, newOrder);

			if (!qstate)
			{
				string checkOrderID_query = "SELECT order_ID FROM orders WHERE order_date = '" + today_date + "' AND cust_ID = '" + custID + "'";
				const char* checkOrderID = checkOrderID_query.c_str();
				qstate = mysql_query(conn, checkOrderID);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					while (row = mysql_fetch_row(res))
						orderID = row[0];
				}
				else
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
		else if (c == 'n' || c == 'N')
		{
			cout << "\nReturning to main menu...\nPress enter to continue...";
			_getch();
			CustomerMainMenu();
		}
		else
		{
			cout << "Invalid input." << endl;
			cout << "Please enter Y for yes or N for no: ";
		}
	} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

	char y;
	do {
		string productID;
		int quantity, new_quantity;
		cout << "Enter product ID: ";
		cin.ignore(1, '\n');
		getline(cin, productID);

		string checkProduct_query = "SELECT * FROM product WHERE product_ID = '" + productID + "'";
		const char* checkProduct = checkProduct_query.c_str();
		qstate = mysql_query(conn, checkProduct);

		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 1)
			{
				string current_quantity, price_per_unit;
				while (row = mysql_fetch_row(res))
				{
					price_per_unit = row[2];
					current_quantity = row[3];
				}

				do {
					if (current_quantity == "0")
					{
						cout << "This product is currently out of stock.\n";
						break;
					}

					cout << "Enter quatity: ";
					cin >> quantity;

					if (quantity > stoi(current_quantity))
					{
						cout << "Invalid quantity. Please enter quantity less than or equal to " << current_quantity << ". \n";
						continue;
					}
					double subtotal;
					subtotal = stod(price_per_unit) * quantity;

					string subtotal_s = to_string(subtotal);
					string quantity_s = to_string(quantity);
					string insertNewOD_query = "INSERT INTO orderdetails (order_ID, product_ID, quantity, sub_total) VALUES ('" + orderID + "', '" + productID + "', '" + quantity_s + "', '" + subtotal_s + "')";
					const char* newOD = insertNewOD_query.c_str();
					qstate = mysql_query(conn, newOD);
					if (qstate)
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

					new_quantity = stoi(current_quantity) - quantity;

					string newQuantity = to_string(new_quantity);
					string updateProductQuantity_query = "UPDATE product SET current_quantity = '" + newQuantity + "' WHERE product_ID = '" + productID + "'";
					const char* UPQ = updateProductQuantity_query.c_str();
					qstate = mysql_query(conn, UPQ);
					if (qstate)
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

				} while (quantity > stoi(current_quantity));
			}
			else
				cout << "Invalid product ID. Try again." << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\nDo you want to order another product? (Y/N):";
		cin >> y;

		while (y != 'y' && y != 'Y' && y != 'n' && y != 'N')
		{
			cout << "Invalid input." << endl;
			cout << "Please enter Y for yes or N for no: ";
			cin >> y;
		}
		if (y == 'n' || y == 'N')
			break;
	} while (y == 'y' || y == 'Y');

	char choice;
	string payment_method, payment_status;
	//payment_method: 1-Cash, 2-Cheque, 3 - Bank-in, 4 - Credit
	//payment_status: 1-Fully paid, 2 - Partially paid, 3 - Pending, 4 - Overdue
	do {
		cout << "Choose your payment method." << endl;
		cout << "1 - Cash\n2 - Cheque\n3 - Bank-in\n4 - Credit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == '1')
			payment_method = "1";
		else if (choice == '2')
			payment_method = "2";
		else if (choice == '3')
			payment_method = "3";
		else if (choice == '4')
			payment_method = "4";
		else
			cout << "Invalid input. Try again\n";

	} while (choice != '1' && choice != '2' && choice != '3' && choice != '4');

	string Dday, Dmonth, Dyear;
	int year, month, day;
	year = 1900 + ltm->tm_year;
	month = 1 + ltm->tm_mon;
	day = ltm->tm_mday;
	int daysOfMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	for (int i = 0; i < 30; i++) {
		if (day < daysOfMonth[month - 1])
			day++;
		else if (day == daysOfMonth[month - 1] && month < 12) {
			day = 1;
			month++;
		}
		else if (day == daysOfMonth[month - 1] && month == 12) {
			day = 1;
			month = 1;
			year++;
		}
	}
	Dday = (day < 10) ? "0" + to_string(day) : to_string(day);
	Dmonth = (month < 10) ? "0" + to_string(month) : to_string(month);
	Dyear = to_string(year);
	string payment_duedate = Dyear + "-" + Dmonth + "-" + Dday;

	payment_status = "3";
	string insertNewPD_query = "INSERT INTO paymentdetails (payment_status, payment_method, payment_duedate, order_ID) VALUES ('" + payment_status + "', '" + payment_method + "', '" + payment_duedate + "', '" + orderID + "')";
	const char* newPD = insertNewPD_query.c_str();
	qstate = mysql_query(conn, newPD);
	if (qstate)
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	double total_amount = 0;
	int total_quantity = 0;
	string checkODgetQtySubT_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "'";
	const char* checkODgetQtySubT = checkODgetQtySubT_query.c_str();
	qstate = mysql_query(conn, checkODgetQtySubT);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			total_quantity += stoi(row[3]);
			total_amount += stod(row[4]);
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	string Tquantity = to_string(total_quantity);
	string Tamount = to_string(total_amount);
	string updateOrders_query = "UPDATE orders SET total_quantity = '" + Tquantity + "', total_amount = '" + Tamount + "' WHERE order_ID = '" + orderID + "'";
	const char* UO = updateOrders_query.c_str();
	qstate = mysql_query(conn, UO);
	if (qstate)
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	Invoice(orderID, custID);
}

void Invoice(string orderID, string custID)
{
	string order_date, payment_duedate, subtotal, productID[50];
	system("cls");
	cout << setw(36) << "INVOICE" << endl << endl;
	cout << "Bill to: ";

	string checkOrderDetail_query = "SELECT * FROM orders WHERE cust_ID = '" + custID + "' AND order_ID = '" + orderID + "'";
	const char* checkOrderD = checkOrderDetail_query.c_str();
	qstate = mysql_query(conn, checkOrderD);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			order_date = row[1];
			subtotal = row[4];
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	string checkPaymentDue_query = "SELECT * FROM paymentdetails WHERE order_ID = '" + orderID + "'";
	const char* checkPD = checkPaymentDue_query.c_str();
	qstate = mysql_query(conn, checkPD);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			payment_duedate = row[5];
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	string checkCustDetails_query = "SELECT * FROM customer WHERE cust_ID = '" + custID + "'";
	const char* checkCustD = checkCustDetails_query.c_str();
	qstate = mysql_query(conn, checkCustD);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t\t\t\tOrder ID: " << orderID << endl << row[1] << "\t\t\t\t\tOrder Date: " << order_date << endl << row[3] << "\t\t\t\t\tPayment Due Date: " << payment_duedate << endl << row[4] << endl << row[5] << endl << row[6] << ", " << row[7] << endl << row[2] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << endl << left << setw(15) << "Product ID" << setw(20) << "Product Name" << setw(15) << "Price/unit" << setw(10) << "Quantity" << endl;

	int i = 0;
	string checkOrdersProduct_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "'";
	const char* checkOP = checkOrdersProduct_query.c_str();
	qstate = mysql_query(conn, checkOP);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			productID[i] = row[2];
			i++;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	for (int j = 0; j < i; j++) {
		string checkProductName_query = "SELECT * FROM product WHERE product_ID = '" + productID[j] + "'";
		const char* checkPN = checkProductName_query.c_str();
		qstate = mysql_query(conn, checkPN);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
				cout << setw(15) << productID[j] << setw(20) << row[1] << setw(15) << row[2];

			string checkProductQuantity_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "' AND product_ID = '" + productID[j] + "'";
			const char* checkPQ = checkProductQuantity_query.c_str();
			qstate = mysql_query(conn, checkPQ);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					cout << setw(10) << row[3] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}

	double sub_total = stod(subtotal);
	double discount, total_amount;
	if (sub_total > 10000) {
		discount = sub_total * 0.05;
		total_amount = sub_total - discount;
	}
	else {
		discount = 0.00;
		total_amount = sub_total;
	}

	cout << "\nSubtotal: " << fixed << setprecision(2) << sub_total << endl;
	cout << "Discount: " << fixed << setprecision(2) << discount << endl;
	cout << "Total Amount: " << fixed << setprecision(2) << total_amount << endl;
	cout << "\n***Please pay within 30 days***";

	string totalAmount = to_string(total_amount);
	string updateTotalAmount_query = "UPDATE orders SET total_amount = '" + totalAmount + "' WHERE order_ID = '" + orderID + "'";
	const char* UTA = updateTotalAmount_query.c_str();
	qstate = mysql_query(conn, UTA);
	if (qstate)
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	_getch();
	cout << "\n\nPress enter to back to main menu.";
	CustomerMainMenu();
}

void StaffMainMenu()
{
	system("cls");
	char choose;
	cout << endl << right << setw(17) << "MAIN MENU" << endl << endl;
	cout << "1 - View Order List\n2 - Update Order\n3 - View Defaulter List\n4 - Add Stock\n5 - View Stock Report\n0 - Exit\n\n";
	cout << "Enter your selection: ";
	cin >> choose;

	if (choose == '1') {
		OrderList();
	}
	else if (choose == '2') {
		UpdateOrder();
	}
	else if (choose == '3') {
		DefaulterList();
	}
	else if (choose == '4') {
		AddStock();
	}
	else if (choose == '5') {
		StockReport();
	}
	else if (choose == '0') {
		cout << "\nLogging out...\n";
		WelcomePage();
	}
	else {
		cout << "Invalid selection.Please try again.";
		StaffMainMenu();
	}
}

void OrderList()
{
	system("cls");
	
	string notPrepared = "1", prepared = "2";
	string checkOrderList_query = "SELECT * FROM orders WHERE delivery_status = '"+notPrepared+"' OR delivery_status = '" + prepared + "'";
	const char* checkOL = checkOrderList_query.c_str();
	qstate = mysql_query(conn, checkOL);
	if (!qstate)
	{
		cout << setw(37) << "ORDER LIST" << endl;
		cout << left << setw(15) << "Order ID" << setw(20) << "Order Date" << setw(15) << "Customer ID" << setw(10) << "Delivery Status" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			if (row[2] == notPrepared)
				cout << left << setw(15) << row[0] << setw(20) << row[1] << setw(15) << row[5] << setw(10) << "Not prepared" << endl;
			else if (row[2] == prepared)
				cout << left << setw(15) << row[0] << setw(20) << row[1] << setw(15) << row[5] << setw(10) << "Prepared" << endl;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char d;
	string orderID;
	do {
		cout << "\nPrint Delivery Order(DO)?  (Y/N):";
		cin >> d;
		if (d == 'y' || d == 'Y') {
			cout << "Enter order ID : ";
			cin.ignore(1, '\n');
			getline(cin, orderID);

			string checkOrderid_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "'";
			const char* checkOrderid = checkOrderid_query.c_str();
			qstate = mysql_query(conn, checkOrderid);
			if (!qstate)
			{
				bool orderFound = false;
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res)) {
					if (orderID == row[1]) {
						orderFound = true;
						break;
					}
				}
				if (orderFound == false) {
					cout << "This order is not exist. Press enter to continue..." << endl;
					_getch();
					StaffMainMenu();
				}
				else
					DeliveryNote(orderID);
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
		else if (d == 'n' || d == 'N')
		{
			cout << "\nReturning to Main Menu...\nPress enter to continue...";
			StaffMainMenu();
		}
		else
			cout << "Invalid input. Try again.\n";
	} while (d != 'y' && d != 'Y' && d != 'n' && d != 'N');
}

void DeliveryNote(string orderID) 
{
	system("cls");
	cout << right << setw(36) << "DELIVERY NOTE" << endl << endl;

	string order_date, cust_ID, productID[50];
	string checkOrderDetail_query = "SELECT * FROM orders WHERE order_ID = '" + orderID + "'";
	const char* checkOrderD = checkOrderDetail_query.c_str();
	qstate = mysql_query(conn, checkOrderD);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			order_date = row[1];
			cust_ID = row[5];
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	
	string checkCustDetails_query = "SELECT * FROM customer WHERE cust_ID = '" + cust_ID + "'";
	const char* checkCust = checkCustDetails_query.c_str();
	qstate = mysql_query(conn, checkCust);
	if (!qstate)
	{
		cout << "Shipping Address: ";
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t\t\tOrder ID: " << orderID << endl << row[1] << "\t\t\t\t\tOrder Date: " << order_date << endl << row[3] << "\t\t\t\t\tDelivery Date: " << today_date << endl << row[4] << endl << row[5] << endl << row[6] << ", " << row[7] << endl << row[2] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	
	cout << endl << left << setw(15) << "Product ID" << setw(20) << "Product Name" << setw(10) << "Quantity" << endl;

	int i = 0;
	string checkOrdersProduct_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "'";
	const char* checkOP = checkOrdersProduct_query.c_str();
	qstate = mysql_query(conn, checkOP);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			productID[i] = row[2];
			i++;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	for (int j = 0; j < i; j++) {
		string checkProductName_query = "SELECT * FROM product WHERE product_ID = '" + productID[j] + "'";
		const char* checkPN = checkProductName_query.c_str();
		qstate = mysql_query(conn, checkPN);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
				cout << setw(15) << productID[j] << setw(20) << row[1];

			string checkProductQuantity_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "' AND product_ID = '" + productID[j] + "'";
			const char* checkPQ = checkProductQuantity_query.c_str();
			qstate = mysql_query(conn, checkPQ);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					cout << setw(10) << row[3] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void UpdateOrder() {}
void DefaulterList() {}
void AddStock() {}
void StockReport() {}

int main() {
	system("cls");
	system("title My Project");
	db_response::ConnectionFunction();
	WelcomePage();
	return 0;
}