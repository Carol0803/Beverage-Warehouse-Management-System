#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <cctype>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <mysql.h>		//libmysql
#include <windows.h>
using namespace std;

int qstate;
MYSQL* conn;	//database connection
MYSQL_ROW row;	//to store one row
MYSQL_RES* res; //return result from query

string user_account_ID, access_type, username;
string today_date;
string now_time;
//int year, month, day;
//payment_method: 1 - Cash, 2-Cheque, 3 - Bank-in, 4 - Credit
//payment_status: 1 - Fully paid, 2 - Partially paid, 3 - Pending, 4 - Overdue
//delivery_status: 1 - Not prepared yet, 2 - Prepared, 3 - Delivered

//void exit(int status);
string getTodayDate();
//string getNowTime();
//string getDueDate(int, int&, int&, int&);
void checkOverdue();
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
//Administrator's Part
void AdminMainMenu();
void AddNewProduct();
void AddNewStaff();
void RemoveStaff();
void CustomerList();
void SalesReport();

//void exit(int status){}

class db_response
{
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (!conn)
			cout << "Failed To Connect!" << endl;

		conn = mysql_real_connect(conn, "localhost", "root", "", "testing", 3306, NULL, 0);
		if (!conn)
		{
			cout << "Failed To Connect To MySql." << endl;
			cout << endl << "Press enter to continue..." << endl;
			_getch();
			system("cls");
		}
	}
};

void header()
{
	string status;

	//cout << "\t------------------------------------------" << endl;
	cout << "\n\tBeverage Warehouse Management System  " << endl;
	//cout << "\t------------------------------------------" << endl;

	if (access_type == "A")
		status = "Administrator";
	else if (access_type == "S")
		status = "Staff";
	else if (access_type == "C")
		status = "Customer";

	cout << "\tUser: " << status << endl;
	cout << "\tUsername: " << username << endl << endl;
	
}

string getTodayDate()
{
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

	return today_date;
}

/*string getNowTime()
{
#pragma warning(disable : 4996)
	time_t today = time(0);
	tm* ltm = localtime(&today);
	int Nsec = ltm->tm_sec;   // seconds of minutes from 0 to 61
	int Nmin = ltm->tm_min;   // minutes of hour from 0 to 59
	int Nhour = ltm->tm_hour;  // hours of day from 0 to 24
	string N_sec = (Nsec < 10) ? "0" + to_string(Nsec) : to_string(Nsec);
	string N_min = (Nmin < 10) ? "0" + to_string(Nmin) : to_string(Nmin);
	string N_hour = (Nhour < 10) ? "0" + to_string(Nhour) : to_string(Nhour);
	string now_time = N_hour + N_min + N_sec;

	return now_time;
}*/

/*string getDueDate(int addDue, int& year, int& month, int& day)
{
	string Dday, Dmonth, Dyear;
	int daysOfMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	for (int i = 0; i < addDue; i++) {
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

	return payment_duedate;
}*/

void checkOverdue()
{
	string orderID;
	string checkOverdue_query = "SELECT * FROM paymentdetails WHERE DATE(payment_duedate) < '" + today_date + "' AND payment_status <> '1'";
	const char* CO = checkOverdue_query.c_str();
	qstate = mysql_query(conn, CO);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			orderID = row[6];
			string updatePaymentStatus_query = "UPDATE paymentdetails SET payment_status = '4' WHERE order_ID = '" + orderID + "'";
			const char* UPS = updatePaymentStatus_query.c_str();
			qstate = mysql_query(conn, UPS);
			if (qstate)
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

/*CreateDirectory(char* DirName, SECURITY_ATTRIBUTES Attribs);

void CreateDirectory(char* DirName, SECURITY_ATTRIBUTES Attribs)
{
	if (!CreateDirectory("c:\\\\mydir", NULL))
		std::cout << "Creating failed";
	else
		std::cout << "Success!";
}*/

void WelcomePage() {
	char selection;
	do {
		system("cls");
		cout << endl << right << setw(45) << "\t------------------------------------------" << endl;
		cout << right << setw(45) << "\t|  Beverage Warehouse Management System  |" << endl;
		cout << right << setw(45) << "\t------------------------------------------" << endl;
		cout << "\n\t1 - Log In\n\t2 - Sign Up\n\t0 - Exit\n\n";
		cout << "\tEnter your selection: ";
		cin >> selection;
		cin.ignore(100, '\n');

		if (selection == '1') {
			Login();
		}
		else if (selection == '2') {
			SignUp();
		}
		else if (selection == '0') {
			cout << "\n\tExiting...\n\tProgram Terminated.\n";
			exit(0);
		}
		else
			cout << "\tInvalid selection.Please try again.";
	} while (selection != '0' && selection != '1' && selection != '2');
}

void Login()
{
	system("cls");
	string userID, password;
	cout << endl << setw(52) << "------------------------------------------" << endl;
	cout << setw(52) << "|  Beverage Warehouse Management System  |" << endl;
	cout << setw(52) << "------------------------------------------" << endl;
	
	cout << endl << right << setw(33) << "LOG IN" << endl;
	cout << "\n\t\tUser ID: ";
	cin >> userID;
	cout << "\t\tPassword: ";
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
				user_account_ID = row[0];
				username = row[1];
				access_type = row[3];

				system("cls");

				if (access_type == "A")
					AdminMainMenu();
				else if (access_type == "S")
					StaffMainMenu();
				else
					CustomerMainMenu();
			}
		}
		else
		{
			char b;
			cout << "\n\n\t\tWrong User ID or Password. \n\t\tDo you want to try again? (Y/N): ";
			do {
				cin >> b;
				cin.ignore(100, '\n');
				if (b == 'y' || b == 'Y')
					Login();
				else if (b == 'n' || b == 'N')
					WelcomePage();
				else
				{
					cout << "\t\tInvalid input." << endl;
					cout << "\t\tPlease enter Y for yes or N for no: ";
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
	cout << endl << setw(52) << "------------------------------------------" << endl;
	cout << setw(52) << "|  Beverage Warehouse Management System  |" << endl;
	cout << setw(52) << "------------------------------------------" << endl;

	string cust_name, cust_tel, cust_add1, cust_add2, cust_area, cust_postcode, cust_state, username, password, password2, userID;
	string access_type = "C";
	cout << endl << "\t" << right << setw(26) << "SIGN UP" << endl << endl;
	cout << "\tPlease fill the details to register an account." << endl;
	//cin.ignore(1, '\n');

	do {
		cout << "\tName: ";
		getline(cin, cust_name);

		if (cust_name.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_name.empty());

	do {
		cout << "\tContact number: ";
		getline(cin, cust_tel);

		if (cust_tel.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_tel.empty());

	do {
		cout << "\tAddress: ";
		getline(cin, cust_add1);

		if (cust_add1.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_add1.empty());

	cout << "\tAddress 2: ";
	getline(cin, cust_add2);

	do {
		cout << "\tArea: ";
		getline(cin, cust_area);

		if (cust_area.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_area.empty());

	do {
		cout << "\tPostcode: ";
		getline(cin, cust_postcode);

		if (cust_postcode.length() != 5)
			cout << "\tPlease follow Malaysia's postcode format. Try again.\n";

		if (cust_postcode.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_postcode.empty() || cust_postcode.length() != 5);

	do {
		cout << "\tState: ";
		getline(cin, cust_state);

		if (cust_state.empty())
			cout << "\tDo not leave blank.\n";
	} while (cust_state.empty());

	do {
		cout << "\tUsername: ";
		getline(cin, username);

		if (username.empty())
			cout << "\tDo not leave blank.\n";
	} while (username.empty());

	do {
		do {
			cout << "\tPassword: ";
			getline(cin, password);

			if (password.empty())
				cout << "\tDo not leave blank.\n";
		} while (password.empty());

		do {
			cout << "\tRe-enter password: ";
			getline(cin, password2);

			if (password2.empty())
				cout << "\tDo not leave blank.\n";
		} while (password2.empty());

		if (password != password2)
			cout << "\tPlease enter the same password to verify. Try again.\n";
	} while (password != password2);

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
			cout << endl << "\tSuccessfully registered!\n";
			cout << "\tYour user ID is " << userID << ".\n";
			cout << "\tPlease log in with your userID and password.\n";
			cout << "\n\tPress enter to continue...";
			_getch();
			WelcomePage();
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void CustomerMainMenu()
{
	char select;
	header();
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\t|       		MAIN MENU  			  |" << endl;
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\tPlease select an option by entering the number labelled.\n" << endl;
	cout << "\t1 - Place order\n\t2 - View Invoice\n\t0 - Log out\n\n";
	cout << "\tEnter your selection: ";
	cin >> select;
	cin.ignore(100, '\n');

	if (select == '1')
		PlaceOrder();
	else if (select == '2') {
		string custid, orderid, order_id;

		system("cls");
		header();
		cout << "\t---------------------------------------------------------" << endl;
		cout << "\t| 			INVOICE	  			|" << endl;
		cout << "\t---------------------------------------------------------" << endl << endl;

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

		cout << "\tOrders: " << endl;
		string checkOrderID_query = "SELECT * FROM orders WHERE cust_ID = '" + custid + "'";
		const char* checkOrderID = checkOrderID_query.c_str();
		qstate = mysql_query(conn, checkOrderID);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
				cout << "\t" << row[0] << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\n\tEnter order ID: ";
		//cin.ignore(1, '\n');
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
				cout << "\tThis order is not exist. Press enter to continue..." << endl;
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
		cout << "\n\tLogging out...\n";
		WelcomePage();
	}
	else {
		cout << "\tInvalid selection.Please try again.";
		CustomerMainMenu();
	}
}

void PlaceOrder()
{
	system("cls");
	header();
	cout << "\t-------------------------------------------------------------------------------------------" << endl;
	cout << "\t|       				PLACE ORDER  					  |" << endl;
	cout << "\t-------------------------------------------------------------------------------------------" << endl << endl;

	string custID, debt;
	string checkCustID_query = "SELECT * FROM customer WHERE user_account_ID = '" + user_account_ID + "'";
	const char* checkCustID = checkCustID_query.c_str();
	qstate = mysql_query(conn, checkCustID);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			custID = row[0];
			debt = row[8];
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	bool overdue = false;
	string checkOverdue_query = "SELECT * FROM customer WHERE cust_ID IN (SELECT cust_ID FROM orders WHERE order_ID IN (SELECT order_ID FROM paymentdetails WHERE payment_status = 4))";
	const char* checkOverdue = checkOverdue_query.c_str();
	qstate = mysql_query(conn, checkOverdue);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			if (row[0] == custID) {
				overdue = true;
				cout << "\tYou are not allowed to place an order due to overdue debt.\n\tDebt amount: RM" << row[8] << "\n\tPlease pay as soon as possible.\n";
			}
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	if (overdue == true)
	{
		cout << "\n\tPress enter to return to Main Menu....";
		_getch();
		system("cls");
		CustomerMainMenu();
	}

	string orderID;
	qstate = mysql_query(conn, "SELECT product_ID, product_name, price_per_unit, current_quantity FROM product");
	if (!qstate)
	{
		cout << right << setw(59) << "PRODUCT LIST" << endl;
		cout << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(15) << "Price/unit" << setw(10) << "Stock Left" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(50) << row[1] << setw(15) << row[2] << setw(10) << row[3] << endl;

		cout << "\n\t*****5% discount for orders over RM10000*****\n\n";
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char c;
	cout << "\tDo you want to order a product? (Y/N): ";
	do {
		cin >> c;
		cin.ignore(1000, '\n');
		if (c == 'y' || c == 'Y')
		{
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
			cout << "\n\tReturning to main menu...\n\tPress enter to continue...";
			_getch();
			system("cls");
			CustomerMainMenu();
		}
		else
		{
			cout << "\tInvalid input." << endl;
			cout << "\tPlease enter Y for yes or N for no: ";
			cout << endl;
		}
	} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

	char y;
	do {
		string productID;
		int quantity, new_quantity;
		cout << "\tEnter product ID: ";
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
						cout << "\tThis product is currently out of stock.\n";
						break;
					}

					cout << "\tEnter quatity: ";
					cin >> quantity;

					if (quantity > stoi(current_quantity))
					{
						cout << "\tInvalid quantity. Please enter quantity less than or equal to " << current_quantity << ". \n";
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
				cout << "\tInvalid product ID. Try again." << endl;
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\n\tDo you want to order another product? (Y/N):";
		cin >> y;
		cin.ignore(100, '\n');
		
		while (y != 'y' && y != 'Y' && y != 'n' && y != 'N')
		{
			cout << "\tInvalid input." << endl;
			cout << "\tPlease enter Y for yes or N for no: ";
			cin >> y;
			cin.ignore(100, '\n');
			cout << endl;
		}
		if (y == 'n' || y == 'N')
			break;
	} while (y == 'y' || y == 'Y');

	string checkOrderItemA_query = "SELECT * FROM orderdetails WHERE order_ID = '" + orderID + "'";
	const char* checkOrderItemA = checkOrderItemA_query.c_str();
	qstate = mysql_query(conn, checkOrderItemA);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 0)
		{
			string deleteOrderID = "DELETE FROM orders WHERE order_ID = '" + orderID + "'";
			const char* DOI = deleteOrderID.c_str();
			qstate = mysql_query(conn, DOI);

			if (qstate)
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			cout << "\n\tReturning to main menu...\n\tPress enter to continue...";
			_getch();
			system("cls");
			CustomerMainMenu();
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char choice;
	string payment_method, payment_status;
	int addDue;
	//payment_method: 1-Cash, 2-Cheque, 3 - Bank-in, 4 - Credit
	//payment_status: 1-Fully paid, 2 - Partially paid, 3 - Pending, 4 - Overdue
	do {
		cout << endl;
		cout << "\tChoose your payment method." << endl;
		cout << "\t1 - Cash\n\t2 - Cheque\n\t3 - Bank-in\n\t4 - Credit\n";
		cout << "\tEnter your choice: ";
		cin >> choice;
		//cin.ignore(1000, '\n');

		if (choice == '1') {
			payment_method = "1";
			addDue = 15;
		}
		else if (choice == '2') {
			payment_method = "2";
			addDue = 15;
		}
		else if (choice == '3') {
			payment_method = "3";
			addDue = 15;
		}
		else if (choice == '4') {
			payment_method = "4";
			addDue = 30;
		}
		else
			cout << "\tInvalid input. Try again\n";
	} while (choice != '1' && choice != '2' && choice != '3' && choice != '4');

	//string payment_duedate = getDueDate(addDue, year, month, day);

#pragma warning(disable : 4996)
	time_t today = time(0);
	tm* ltm = localtime(&today);

	string Dday, Dmonth, Dyear;
	int year, month, day;
	year = 1900 + ltm->tm_year;
	month = 1 + ltm->tm_mon;
	day = ltm->tm_mday;
	int daysOfMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	for (int i = 0; i < addDue; i++) {
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
	
	double T_debt = 0;
	T_debt = stod(debt) + total_amount;
	string Tdebt = to_string(T_debt);
	string updateDebt_query = "UPDATE customer SET cust_debt_amount = '" + Tdebt + "' WHERE cust_ID = '" + custID + "'";
	const char* UD = updateDebt_query.c_str();
	qstate = mysql_query(conn, UD);
	if (qstate)
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	Invoice(orderID, custID);
}

void Invoice(string orderID, string custID)
{
	string order_date, payment_duedate, subtotal, productID[50];
	system("cls");
	header();
	cout << "\t--------------------------------------------------------------------------------------------" << endl << endl;
	cout << right << setw(50) << "INVOICE" << endl << endl;
	cout << "\t" << left << setw(60) << "Bill to: ";

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
			cout << left << "Order ID: " << orderID << endl << "\t" << setw(60) << row[1] << "Order Date: " << order_date << endl << "\t" << setw(60) << row[3] << "Payment Due Date: " << payment_duedate  << endl << "\t" << row[4] << endl << "\t" << row[5] << endl << "\t" << row[6] << ", " << row[7] << endl << "\t" << row[2] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << endl << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(15) << "Price/unit" << setw(10) << "Quantity" << endl;

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
				cout << "\t" << setw(15) << productID[j] << setw(50) << row[1] << setw(15) << row[2];

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

	cout << "\n\tSubtotal: " << fixed << setprecision(2) << sub_total << endl;
	cout << "\tDiscount: " << fixed << setprecision(2) << discount << endl;
	cout << "\tTotal Amount: " << fixed << setprecision(2) << total_amount << endl;
	cout << "\n\t*****Please pay before " << payment_duedate << "*****";

	char txt;
	do {
		cout << "\n\n\tDo you want to export as text file?(Y/N): ";
		cin >> txt;
		cin.ignore(1000, '\n');

		if (txt == 'Y' || txt == 'y') {
			CreateDirectory(L"C:\\Beverage Warehouse Management System\\Invoice", NULL);
			string filename = "C:\\Beverage Warehouse Management System\\Invoice\\INV" + orderID + ".txt";
			ofstream writeFile(filename, ios::out);
			auto cout_buff = cout.rdbuf();	// pointer to cout buffer
			cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

			cout << right << setw(50) << "INVOICE" << endl << endl;
			cout << left << setw(60) << "Bill to: ";

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
					cout << left << "Order ID: " << orderID << endl << setw(60) << row[1] << "Order Date: " << order_date << endl << setw(60) << row[3] << "Payment Due Date: " << payment_duedate << endl << row[4] << endl << row[5] << endl << row[6] << ", " << row[7] << endl << row[2] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			cout << endl << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(15) << "Price/unit" << setw(10) << "Quantity" << endl;

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
						cout << setw(15) << productID[j] << setw(50) << row[1] << setw(15) << row[2];

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
			cout << "\n*****Please pay before " << payment_duedate << "*****";

			cout.rdbuf(cout_buff);	// go back to cout buffer
			cout << "\tSuccessfully exported to ";
			cout << filename << endl;
		}
		else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
			cout << "\tInvalid input.Try again.";
	} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');

	string totalAmount = to_string(total_amount);
	string updateTotalAmount_query = "UPDATE orders SET total_amount = '" + totalAmount + "' WHERE order_ID = '" + orderID + "'";
	const char* UTA = updateTotalAmount_query.c_str();
	qstate = mysql_query(conn, UTA);
	if (qstate)
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << "\n\tPress enter to back to main menu...";
	_getch();
	system("cls");
	CustomerMainMenu();
}

void StaffMainMenu()
{
	char choose;
	header();
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\t|       		MAIN MENU  			  |" << endl;
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\tPlease select an option by entering the number labelled.\n" << endl;
	cout << "\t1 - View Order List\n\t2 - Update Order\n\t3 - View Defaulter List\n\t4 - Add Stock\n\t5 - View Stock Report\n\t0 - Log Out\n\n";
	cout << "\tEnter your selection: ";
	cin >> choose;
	cin.ignore(100, '\n');

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
		cout << "\n\tLogging out...\n";
		WelcomePage();
		//exit(0);
	}
	else {
		cout << "\tInvalid selection.Please try again.";
		system("cls");
		StaffMainMenu();
	}
}

void OrderList()
{
	system("cls");
	header();

	string notPrepared = "1", prepared = "2";
	string checkOrderList_query = "SELECT * FROM orders WHERE delivery_status = '" + notPrepared + "' OR delivery_status = '" + prepared + "'";
	const char* checkOL = checkOrderList_query.c_str();
	qstate = mysql_query(conn, checkOL);
	if (!qstate)
	{
		cout << "\t----------------------------------------------------------------" << endl;
		cout << "\t|           		   ORDER LIST  			       |" << endl;
		cout << "\t----------------------------------------------------------------" << endl << endl;
		cout << "\t" << left << setw(15) << "Order ID" << setw(15) << "Order Date" << setw(17) << "Customer ID" << setw(10) << "Delivery Status" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res)) {
			if (row[2] == notPrepared)
				cout << "\t" << left << setw(15) << row[0] << setw(15) << row[1] << setw(17) << row[5] << setw(10) << "Not prepared" << endl;
			else if (row[2] == prepared)
				cout << "\t" << left << setw(15) << row[0] << setw(15) << row[1] << setw(17) << row[5] << setw(10) << "Prepared" << endl;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char d;
	string orderID;
	do {
		cout << "\n\tPrint Delivery Order(DO)?  (Y/N):";
		cin >> d;
		cin.ignore(100, '\n');
		if (d == 'y' || d == 'Y') {
			cout << "\tEnter order ID : ";
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
					cout << "\tThis order is not exist. Press enter to continue..." << endl;
					_getch();
					system("cls");
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
			cout << "\n\tReturning to Main Menu...\n\tPress enter to continue...";
			_getch();
			system("cls");
			StaffMainMenu();
		}
		else
			cout << "\tInvalid input. Try again.\n";
	} while (d != 'y' && d != 'Y' && d != 'n' && d != 'N');
}

void DeliveryNote(string orderID)
{
	system("cls");
	header();
	cout << "\t--------------------------------------------------------------------------------------------------" << endl << endl;
	cout << "\t" << right << setw(45) << "DELIVERY NOTE" << endl << endl;

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
		cout << "\t" << left << setw(60) << "Shipping Address: ";
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << left << "Order ID: " << orderID << endl << "\t" << setw(60) << row[1] << "Order Date: " << order_date << endl << "\t" << setw(60) << row[3] << "Delivery Date: " << today_date << endl << "\t" << row[4] << endl << "\t" << row[5] << endl << "\t" << row[6] << ", " << row[7] << endl << "\t" << row[2] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << endl << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(10) << "Quantity" << endl;

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
				cout << "\t" << setw(15) << productID[j] << setw(50) << row[1];

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

	char txt;
	do {
		cout << "\n\n\tDo you want to export as text file?(Y/N): ";
		cin >> txt;
		cin.ignore(100, '\n');

		if (txt == 'Y' || txt == 'y') {
			CreateDirectory(L"C:\\Beverage Warehouse Management System\\Delivery Note", NULL);
			string filename = "C:\\Beverage Warehouse Management System\\Delivery Note\\DO" + orderID + ".txt";
			ofstream writeFile(filename, ios::out);
			auto cout_buff = cout.rdbuf();	// pointer to cout buffer
			cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

			cout << right << setw(45) << "DELIVERY NOTE" << endl << endl;

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
				cout << left << setw(60) << "Shipping Address: ";
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					cout << left << "Order ID: " << orderID << endl << setw(60) << row[1] << "Order Date: " << order_date << endl << setw(60) << row[3] << "Delivery Date: " << today_date << endl << row[4] << endl << row[5] << endl << row[6] << ", " << row[7] << endl << row[2] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			cout << endl << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(10) << "Quantity" << endl;

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
						cout << setw(15) << productID[j] << setw(50) << row[1];

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

			cout.rdbuf(cout_buff);	// go back to cout buffer
			cout << "\tSuccessfully exported to ";
			cout << filename << endl;
		}
		else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
			cout << "\tInvalid input.Try again.";
	} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');

	cout << "\n\tPress enter to return to Main Menu...";
	_getch();
	system("cls");
	StaffMainMenu();
}

void UpdateOrder()
{
	string orderID, new_delivery_status, new_payment_status;
	bool orderFound = false;

	system("cls");
	header();
	cout << "\t----------------------------------------------------------------" << endl;
	cout << "\t|		  	   UPDATE ORDER  		       |" << endl;
	cout << "\t----------------------------------------------------------------" << endl << endl;
	cout << "\t1 - Update Delivery Status\n\t2 - Update Payment Status\n\t0 - Return to Main Menu\n\n";

	char ch;
	cout << "\tEnter your choice: ";
	cin >> ch;
	cin.ignore(1000, '\n');

	if (ch == '1')
	{
		system("cls");
		header();
		cout << "\t-----------------------------------------" << endl;
		cout << "\t|	 UPDATE DELIVERY STATUS		|" << endl;
		cout << "\t-----------------------------------------" << endl << endl;

		string ds, DS;
		qstate = mysql_query(conn, "SELECT * FROM orders WHERE delivery_status <> '3'");
		if (!qstate)
		{
			cout << "\t" << left << setw(15) << "Orders" << setw(15) << "Status" << endl;
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				DS = row[2];
				if (DS == "1")
					ds = "Not prepared yet";
				else if (DS == "2")
					ds = "Prepared";
				cout << "\t" << left << setw(15) << row[0] << setw(30) << ds << endl;
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\n\tEnter Order ID: ";
		getline(cin, orderID);

		string checkOrder_query = "SELECT * FROM orders WHERE order_ID = '" + orderID + "'";
		const char* checkOrder = checkOrder_query.c_str();
		qstate = mysql_query(conn, checkOrder);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 1)
			{
				cout << "\n\tDelivery Status: \n\t1 - Not prepared\n\t2 - Prepared\n\t3 - Delivered\n\tUpdate the delivery status to: ";
				cin >> new_delivery_status;

				do {
					if (new_delivery_status == "1" || new_delivery_status == "2" || new_delivery_status == "3")
					{
						string updateDeliveryStatus_query = "UPDATE orders SET delivery_status = '" + new_delivery_status + "'WHERE order_ID = '" + orderID + "'";
						const char* UDS = updateDeliveryStatus_query.c_str();
						qstate = mysql_query(conn, UDS);
						if (qstate)
							cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
						else
						{
							cout << "\n\tSuccessfully updated.\n";
							cout << "\tPress enter to return to Main Menu...";
							_getch();
							system("cls");
							StaffMainMenu();
						}
					}
					else
					{
						cout << "\tInvalid input. \n\tPress enter to try again...\n";
						_getch();
						UpdateOrder();
					}
				} while (new_delivery_status != "1" && new_delivery_status != "2" && new_delivery_status != "3");
			}
			else {
				cout << "\tThis order is not exist. Press enter to try again..." << endl;
				_getch();
				UpdateOrder();
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	else if (ch == '2')
	{
		system("cls");
		header();
		cout << "\t---------------------------------------------------------" << endl;
		cout << "\t|		 UPDATE PAYMENT STATUS			|" << endl;
		cout << "\t---------------------------------------------------------" << endl << endl;

		string ps, PS, amount_paid, amount[1000];
		double amount_unpaid[1000], amounts[1000];
		int i = 0;
		qstate = mysql_query(conn, "SELECT * FROM orders WHERE order_ID IN (SELECT order_ID FROM paymentdetails WHERE payment_status <> '1')");
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				amount[i] = row[4];
				amounts[i] = stod(amount[i]);
				i++;
			}
			//mysql_free_result(res);
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		int j = 0;
		qstate = mysql_query(conn, "SELECT * FROM paymentdetails WHERE payment_status <> '1'");
		if (!qstate)
		{
			cout << "\t" << left << setw(15) << "Orders" << setw(20) << "Status" << setw(15) << "Amount Unpaid" << endl;
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				PS = row[2];
				amount_paid = row[4];
				amount_unpaid[j] = amounts[j] - stod(amount_paid);
				if (PS == "2")
					ps = "Partially Paid";
				else if (PS == "3")
					ps = "Pending";
				else if (PS == "4")
					ps = "Overdue";
				cout << "\t" << left << setw(15) << row[6] << setw(20) << ps << setw(15) << fixed << setprecision(2) << amount_unpaid[j] << endl;
				j++;
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << endl;
		cout << "\tEnter Order ID: ";
		//cin.ignore(1, '\n');
		getline(cin, orderID);

		string total_amount;
		string CO_query = "SELECT * FROM orders WHERE order_ID = '" + orderID + "'";
		const char* CO = CO_query.c_str();
		qstate = mysql_query(conn, CO);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 1)
			{
				row = mysql_fetch_row(res);
				total_amount = row[4];
				cout << "\n\tPayment Status: \n\t1 - Fully paid\n\t2 - Partially paid\n\tUpdate the payment status to: ";
				cin >> new_payment_status;

				do {
					if (new_payment_status == "1" || new_payment_status == "2")
					{
						string paymentStatus, amountPaid;
						string ps_query = "SELECT * FROM paymentdetails WHERE order_ID = '" + orderID + "'";
						const char* ps = ps_query.c_str();
						qstate = mysql_query(conn, ps);
						if (!qstate)
						{
							res = mysql_store_result(conn);
							if (res->row_count == 1) {
								row = mysql_fetch_row(res);
								paymentStatus = row[2];
								amountPaid = row[4];
							}
						}
						else
							cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

						if (paymentStatus == "4" && new_payment_status == "2")
							new_payment_status = "4";

						string updateNewPaymentStatus_query = "UPDATE paymentdetails SET payment_status = '" + new_payment_status + "'WHERE order_ID = '" + orderID + "'";
						const char* UNPS = updateNewPaymentStatus_query.c_str();
						qstate = mysql_query(conn, UNPS);
						if (qstate)
							cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
						else
						{
							string custID, debt;
							string checkCustID_query = "SELECT * FROM customer WHERE cust_ID = (SELECT cust_ID FROM orders WHERE order_ID = '" + orderID + "')";
							const char* checkCustID = checkCustID_query.c_str();
							qstate = mysql_query(conn, checkCustID);
							if (!qstate)
							{
								res = mysql_store_result(conn);
								while (row = mysql_fetch_row(res)) {
									custID = row[0];
									debt = row[8];
								}
							}
							else
								cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

							double newDebt, new_paid_amount, paid_amount = 0;
							if (new_payment_status == "1")
							{
								newDebt = stod(debt) - (stod(total_amount) - stod(amountPaid));
								string new_debt = to_string(newDebt);
								string updateDebt1_query = "UPDATE customer SET cust_debt_amount = '" + new_debt + "' WHERE cust_ID = '" + custID + "'";
								const char* UD1 = updateDebt1_query.c_str();
								qstate = mysql_query(conn, UD1);
								if (qstate)
									cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

								string updatePD1_query = "UPDATE paymentdetails SET payment_date = '" + today_date + "', amount_paid = '" + total_amount + "' WHERE order_ID = '" + orderID + "'";
								const char* UPD1 = updatePD1_query.c_str();
								qstate = mysql_query(conn, UPD1);
								if (qstate)
									cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
							}
							else
							{
								cout << "\tEnter amount paid: RM ";
								cin >> paid_amount;
								new_paid_amount = paid_amount + stod(amountPaid);
								newDebt = stod(debt) - paid_amount;
								string new_debt = to_string(newDebt);
								string updateDebt_query = "UPDATE customer SET cust_debt_amount = '" + new_debt + "' WHERE cust_ID = '" + custID + "'";
								const char* UD = updateDebt_query.c_str();
								qstate = mysql_query(conn, UD);
								if (qstate)
									cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

								string newPaidAmount = to_string(new_paid_amount);
								string updatePD_query = "UPDATE paymentdetails SET payment_date = '" + today_date + "', amount_paid = '" + newPaidAmount + "' WHERE order_ID = '" + orderID + "'";
								const char* UPD = updatePD_query.c_str();
								qstate = mysql_query(conn, UPD);
								if (qstate)
									cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
							}

							cout << "\n\tSuccessfully updated.\n";
							cout << "\tPress enter to return to Main Menu...";
							_getch();
							system("cls");
							StaffMainMenu();
						}
					}
					else
					{
						cout << "\tInvalid input. \n\tPress enter to try again...\n";
						_getch();
						UpdateOrder();
					}
				} while (new_payment_status != "1" && new_payment_status != "2");
			}
			else {
				cout << "\n\tThis order is not exist. \n\tPress enter to try again..." << endl;
				_getch();
				UpdateOrder();
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	else if (ch == '0')
	{
		cout << "\n\tReturning to Main Menu...\n\tPress enter to continue...\n";
		_getch();
		system("cls");
		StaffMainMenu();
	}
	else
	{
		cout << "\tInvalid choice. Press enter to try again." << endl;
		_getch();
		UpdateOrder();
	}
}

void DefaulterList()
{
	system("cls");
	header();
	cout << "\t---------------------------------------------------------------------------------------" << endl;
	cout << "\t|			 	   DEFAULTER LIST  				      |" << endl;
	cout << "\t---------------------------------------------------------------------------------------" << endl << endl;
	

	string checkOverdue_query = "SELECT * FROM customer WHERE cust_ID IN (SELECT cust_ID FROM orders WHERE order_ID IN (SELECT order_ID FROM paymentdetails WHERE payment_status = 4))";
	const char* checkOverdue = checkOverdue_query.c_str();
	qstate = mysql_query(conn, checkOverdue);
	if (!qstate)
	{
		cout << "\t" << left << setw(15) << "Customer ID" << setw(25) << "Customer Name" << setw(20) << "Customer Tel No." << setw(25) << "Customer's Debt Amount(RM)" << endl;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(25) << row[1] << setw(20) << row[2] << setw(25) << fixed << setprecision(2) << right << row[8] << endl;

		char txt;
		do {
			cout << "\n\n\tDo you want to export as text file?(Y/N): ";
			cin >> txt;
			cin.ignore(100, '\n');

			if (txt == 'Y' || txt == 'y') {
				CreateDirectory(L"C:\\Beverage Warehouse Management System\\Defaulter List", NULL);
				string filename = "C:\\Beverage Warehouse Management System\\Defaulter List\\Defaulter List " + today_date + ".txt";
				ofstream writeFile(filename, ios::out);
				auto cout_buff = cout.rdbuf();	// pointer to cout buffer
				cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

				cout << right << setw(50) << "DEFAULTER LIST" << endl << endl;

				string checkOverdue_query = "SELECT * FROM customer WHERE cust_ID IN (SELECT cust_ID FROM orders WHERE order_ID IN (SELECT order_ID FROM paymentdetails WHERE payment_status = 4))";
				const char* checkOverdue = checkOverdue_query.c_str();
				qstate = mysql_query(conn, checkOverdue);
				if (!qstate)
				{
					cout << left << setw(15) << "Customer ID" << setw(25) << "Customer Name" << setw(20) << "Customer Tel No." << setw(25) << "Customer's Debt Amount(RM)" << endl;
					res = mysql_store_result(conn);
					while (row = mysql_fetch_row(res))
						cout << left << setw(15) << row[0] << setw(25) << row[1] << setw(20) << row[2] << setw(25) << fixed << setprecision(2) << right << row[8] << endl;
				}
				else
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

				cout.rdbuf(cout_buff);	// go back to cout buffer
				cout << "\tSuccessfully exported to ";
				cout << filename << endl;
			}
			else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
				cout << "\tInvalid input.Try again.";
		} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');

		cout << "\n\tPress enter to return to Main Menu....";
		_getch();
		system("cls");
		StaffMainMenu();
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void AddStock()
{
	system("cls");
	header();
	cout << "\t----------------------------------------------------------------------------" << endl;
	cout << "\t|			   	  ADD STOCK 				   |" << endl;
	cout << "\t----------------------------------------------------------------------------" << endl << endl;

	qstate = mysql_query(conn, "SELECT * FROM product");
	if (!qstate)
	{
		cout << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(10) << "Stock Left" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(50) << row[1] << setw(10) << row[3] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	cout << endl;

	string productID, current_quantity, added_quantity;
	int quantity, add_quantity;
	char f;

	do {
		cout << "\tEnter product ID: ";
		cin.ignore(1, '\n');
		getline(cin, productID);

		string checkProduct_query = "SELECT * FROM product WHERE product_ID = '" + productID + "'";
		const char* checkProduct = checkProduct_query.c_str();
		qstate = mysql_query(conn, checkProduct);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 0)
			{
				cout << "\tThis product does not exist.\n";
			}
			while (row = mysql_fetch_row(res))
			{
				current_quantity = row[3];
				cout << "\tEnter the replenishment quantity: ";
				cin >> quantity;
				add_quantity = stoi(current_quantity) + quantity;

				added_quantity = to_string(add_quantity);
				string updateQuantity_query = "UPDATE product SET current_quantity = '" + added_quantity + "'WHERE product_ID = '" + productID + "'";
				const char* UQ = updateQuantity_query.c_str();
				qstate = mysql_query(conn, UQ);
				if (qstate)
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
				else
					cout << "\tSuccessfully restock!\n";
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		do {
			cout << "\tContinue?(Y/N): ";
			cin >> f;
			cin.ignore(100, '\n');

			if (f == 'n' || f == 'N')
			{
				cout << "\n\tPress enter to return to Main Menu....";
				_getch();
				system("cls");
				StaffMainMenu();
				break;
			}

			if (f != 'n' && f != 'N' && f != 'y' && f != 'Y')
				cout << "\tInvalid input. Try again.\n";

		} while (f != 'n' && f != 'N' && f != 'y' && f != 'Y');
		cout << endl;

	} while (f == 'y' || f == 'Y');
}

/*string GetCurrentDirectoryA()
{
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);
	return string(buffer);
}*/

/*void ExportTxtFile(string filename, int action)
{
	ofstream writeFile(filename, ios::out);
	auto cout_buff = cout.rdbuf();

	if (action == 1) {

		//https://www.quora.com/How-do-I-output-all-my-cout-s-to-a-text-file-in-C

		cout.rdbuf(writeFile.rdbuf());
		//https://en.cppreference.com/w/cpp/io/basic_ios/rdbuf
		// other:
		//		http://www.cplusplus.com/forum/general/19489/
	}
	if (action == 2)
		cout.rdbuf(cout_buff);
}*/

void StockReport()
{
	system("cls");
	header();
	cout << "\t------------------------------------------------------------------------------------" << endl;
	cout << "\t|			   	   STOCK REPORT					   |" << endl;
	cout << "\t------------------------------------------------------------------------------------" << endl;
	cout << "\t 			   	 " << "DATE: " << today_date << " 				    " << endl <<endl;

	qstate = mysql_query(conn, "SELECT * FROM product");
	if (!qstate)
	{
		cout << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(10) << "Stock Left" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(50) << row[1] << setw(10) << row[3] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	qstate = mysql_query(conn, "SELECT * FROM product");
	if (!qstate)
	{
		string quantity, reorder_level;
		int qty, rl;

		cout << "\n\n\t********STOCK REMINDER********\n";
		cout << "\tItems that need replenishment:\n\n";
		cout << "\t" << left << setw(15) << "Product ID" << setw(50) << "Product Name" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			quantity = row[3];
			reorder_level = row[4];
			qty = stoi(quantity);
			rl = stoi(reorder_level);

			if (qty < rl)
				cout << "\t" << left << setw(15) << row[0] << setw(50) << row[1] << endl;
		}
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char txt;
	do {
		cout << "\n\tDo you want to export as text file?(Y/N): ";
		cin >> txt;
		cin.ignore(1000, '\n');

		if (txt == 'Y' || txt == 'y') {
			CreateDirectory(L"C:\\Beverage Warehouse Management System\\Stock Report", NULL);
			string filename = "C:\\Beverage Warehouse Management System\\Stock Report\\Stock Report " + today_date + ".txt";
			ofstream writeFile(filename, ios::out);
			auto cout_buff = cout.rdbuf();	// pointer to cout buffer
			cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

			cout << right << setw(45) << "STOCK REPORT\n";
			cout << right << setw(37) << "DATE: " << today_date << endl << endl;

			qstate = mysql_query(conn, "SELECT * FROM product");
			if (!qstate)
			{
				cout << left << setw(15) << "Product ID" << setw(50) << "Product Name" << setw(10) << "Stock Left(carton)" << endl;

				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					cout << left << setw(15) << row[0] << setw(50) << row[1] << setw(10) << row[3] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			qstate = mysql_query(conn, "SELECT * FROM product");
			if (!qstate)
			{
				string quantity, reorder_level;
				int qty, rl;

				cout << "\n\n********STOCK REMINDER********\n";
				cout << "Items that need replenishment:\n\n";
				cout << left << setw(15) << "Product ID" << setw(50) << "Product Name" << endl;

				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					quantity = row[3];
					reorder_level = row[4];
					qty = stoi(quantity);
					rl = stoi(reorder_level);

					if (qty < rl)
						cout << left << setw(15) << row[0] << setw(50) << row[1] << endl;
				}
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			cout.rdbuf(cout_buff);	// go back to cout buffer
			cout << "\tSuccessfully exported to ";
			cout << filename << endl;
		}
		else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
			cout << "\tInvalid input.Try again.\n";
	} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');

	string position;
	string checkStaffPosition_query = "SELECT * FROM useraccount WHERE user_account_ID = '" + user_account_ID + "'";
	const char* checkStaffPosition = checkStaffPosition_query.c_str();
	qstate = mysql_query(conn, checkStaffPosition);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			position = row[3];
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << "\n\tPress enter to return to Main Menu....";
	_getch();
	system("cls");
	if (position == "S")
		StaffMainMenu();
	if (position == "A")
		AdminMainMenu();
}

void AdminMainMenu()
{
	char choices;
	header();
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\t|       		MAIN MENU  			  |" << endl;
	cout << "\t-----------------------------------------------------------" << endl;
	cout << "\tPlease select an option by entering the number labelled.\n" << endl;
	cout << "\t1 - Add New Product\n\t2 - Add Staff\n\t3 - Remove Staff\n\t4 - View Customer List\n\t5 - View Stock Report\n\t6 - View Sales Report\n\t0 - Log Out\n\n";
	cout << "\tEnter your selection: ";
	cin >> choices;
	cin.ignore(100, '\n');

	if (choices == '1') {
		AddNewProduct();
	}
	else if (choices == '2') {
		AddNewStaff();
	}
	else if (choices == '3') {
		RemoveStaff();
	}
	else if (choices == '4') {
		CustomerList();
	}
	else if (choices == '5') {
		StockReport();
	}
	else if (choices == '6') {
		SalesReport();
	}
	else if (choices == '0') {
		cout << "\n\tLogging out...\n";
		WelcomePage();
		//exit(0);
	}
	else {
		cout << "\tInvalid selection.Please try again.";
		system("cls");
		AdminMainMenu();
	}
}

void AddNewProduct()
{
	system("cls");
	header();
	cout << "\t-----------------------------------------------------" << endl;
	cout << "\t|		    ADD NEW PRODUCT		    |" << endl;
	cout << "\t-----------------------------------------------------" << endl << endl;
	string productID, product_name, price, reorder_level;

	bool allNotEmpty = false;
	char np;

	do {
		do {
			cout << "\tEnter product name: ";
			//cin.ignore(1, '\n');
			getline(cin, product_name);

			if (product_name.empty())
				cout << "\tDo not leave blank.\n";
		} while (product_name.empty());

		do {
			cout << "\tSet price per unit: RM";
			getline(cin, price);

			if (price.empty())
				cout << "\tDo not leave blank.\n";
		} while (price.empty());

		do {
			cout << "\tSet reorder level(carton): ";
			getline(cin, reorder_level);

			if (reorder_level.empty())
				cout << "\tDo not leave blank.\n";
			else
				allNotEmpty = true;

		} while (reorder_level.empty());

		if (allNotEmpty == true) {
			string insertNewProduct_query = "INSERT INTO product (product_name, price_per_unit, reorder_level) values ('" + product_name + "', '" + price + "', '" + reorder_level + "')";
			const char* newP = insertNewProduct_query.c_str();
			qstate = mysql_query(conn, newP);

			if (qstate)
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			else
			{
				string checkProductID_query = "SELECT product_ID FROM product WHERE product_name = '" + product_name + "'";
				const char* checkPID = checkProductID_query.c_str();
				qstate = mysql_query(conn, checkPID);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					while (row = mysql_fetch_row(res))
						cout << "\n\tNew product successfully added! The product ID is " << row[0];

					do {
						cout << "\n\tIs there another new product? (Y/N): ";
						cin >> np;
						cin.ignore(100, '\n');

						if (np != 'y' && np != 'Y' && np != 'n' && np != 'N')
							cout << "\tInvalid input.";
						if (np == 'n' || np == 'N') {
							cout << "\n\tPress enter to return to Main Menu....";
							_getch();
							system("cls");
							AdminMainMenu();
						}
					} while (np != 'y' && np != 'Y' && np != 'n' && np != 'N');
				}
				else
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			}
		}
		cout << endl;
	} while (np == 'y' || np == 'Y');
}

void AddNewStaff()
{
	system("cls");
	header();
	cout << "\t---------------------------------------------------------------------" << endl;
	cout << "\t|			    ADD NEW STAFF			    |" << endl;
	cout << "\t---------------------------------------------------------------------" << endl << endl;

	string access_type, s_name, s_tel, s_add1, s_add2, s_area, s_postcode, s_state, username, password, password2, userID;

	cout << "\tPlease fill in the details to register a new staff." << endl << endl;
	do {
		cout << "\tRegister as? (S-Staff, A-Administrator): ";
		getline(cin, access_type);

		if (access_type == "s")
			access_type = "S";
		if (access_type == "a")
			access_type = "A";
		if (access_type.empty())
			cout << "\tDo not leave blank.\n";

		if (access_type != "S" && access_type != "A" && access_type != "s" && access_type != "A" && !access_type.empty())
			cout << "\tInvalid input.Try again.\n";
	} while (access_type.empty() || (access_type != "S" && access_type != "A" && access_type != "s" && access_type != "A"));

	do {
		cout << "\tName: ";
		//cin.ignore(1, '\n');
		getline(cin, s_name);

		if (s_name.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_name.empty());

	do {
		cout << "\tContact number: ";
		getline(cin, s_tel);

		if (s_tel.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_tel.empty());

	do {
		cout << "\tAddress: ";
		getline(cin, s_add1);

		if (s_add1.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_add1.empty());

	cout << "\tAddress 2: ";
	getline(cin, s_add2);

	do {
		cout << "\tArea: ";
		getline(cin, s_area);

		if (s_area.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_area.empty());

	do {
		cout << "\tPostcode: ";
		getline(cin, s_postcode);

		if (s_postcode.length() != 5)
			cout << "\tPlease follow Malaysia's postcode format. Try again.\n";

		if (s_postcode.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_postcode.empty() || s_postcode.length() != 5);

	do {
		cout << "\tState: ";
		getline(cin, s_state);

		if (s_state.empty())
			cout << "\tDo not leave blank.\n";
	} while (s_state.empty());

	do {
		cout << "\tUsername: ";
		getline(cin, username);

		if (username.empty())
			cout << "\tDo not leave blank.\n";
	} while (username.empty());

	do {
		do {
			cout << "\tPassword: ";
			getline(cin, password);

			if (password.empty())
				cout << "\tDo not leave blank.\n";
		} while (password.empty());

		do {
			cout << "\tRe-enter password: ";
			getline(cin, password2);

			if (password2.empty())
				cout << "\tDo not leave blank.\n";
		} while (password2.empty());

		if (password != password2)
			cout << "\tPlease enter the same password to verify. Try again.\n";
	} while (password != password2);

	string insertNewStaffAcc_query = "INSERT INTO useraccount (username, password, access_type) values ('" + username + "', '" + password + "', '" + access_type + "')";
	const char* newSA = insertNewStaffAcc_query.c_str();
	qstate = mysql_query(conn, newSA);

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

		string insertNewStaff_query = "INSERT INTO staff (staff_name, staff_tel, staff_address1, staff_address2, staff_area, staff_postcode, staff_state, user_account_ID) VALUES ('" + s_name + "', '" + s_tel + "', '" + s_add1 + "', '" + s_add2 + "', '" + s_area + "', '" + s_postcode + "', '" + s_state + "', '" + userID + "')";
		const char* newStaff = insertNewStaff_query.c_str();
		qstate = mysql_query(conn, newStaff);

		if (!qstate)
		{
			cout << endl << "\tNew staff successfully added!\n";
			cout << "\tNew staff's user ID is " << userID << ".\n";
			cout << "\tNew staff can login the system with this user ID and password.\n";
			cout << "\n\tPress enter to return to Main Menu...";
			getch();
			system("cls");
			AdminMainMenu();
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void RemoveStaff()
{
	system("cls");
	header();
	cout << "\t-----------------------------------------------------" << endl;
	cout << "\t|		    REMOVE STAFF		    |" << endl;
	cout << "\t-----------------------------------------------------" << endl << endl;

	string staffID, staff_name, userID;

	qstate = mysql_query(conn, "SELECT staff_ID, staff_name FROM staff");
	if (!qstate)
	{
		cout << "\t" << left << setw(15) << "StaffID" << setw(50) << "Name" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(50) << row[1] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	char p;
	do {
		qstate = mysql_query(conn, "SELECT staff_ID FROM staff");
		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 0) {
				cout << "\tNo staff to be removed.\n";
				cout << "\n\tPress enter to back to Main Menu...";
				_getch();
				system("cls");
				AdminMainMenu();
			}
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

		cout << "\n\tEnter staff ID: ";
		cin.ignore(1, '\n');
		getline(cin, staffID);

		string checkStaff_query = "SELECT * FROM staff WHERE staff_ID = '" + staffID + "'";
		const char* checkStaff = checkStaff_query.c_str();
		qstate = mysql_query(conn, checkStaff);

		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (res->row_count == 0)
			{
				do
				{
					cout << "\tThis staff ID is not valid.";
					cout << "\n\tContinue? (Y/N): ";
					cin >> p;
					cin.ignore(100, '\n');
					if (p == 'n' || p == 'N')
					{
						cout << "\n\tPress enter to back to Main Menu...";
						_getch();
						system("cls");
						AdminMainMenu();
					}
					else if (p == 'y' || p == 'Y')
						continue;
					else
						cout << "\tInvalid input.\n\n";
				} while (p != 'y' && p != 'Y' && p != 'n' && p != 'N');
			}

			while (row = mysql_fetch_row(res)) {
				staff_name = row[1];
				userID = row[8];
			}
			char r;
			do {
				cout << "\tAre you sure you want to remove " << staff_name << " ? (Y/N):";
				cin >> r;
				cin.ignore(100, '\n');
				if (r == 'y' || r == 'Y')
				{
					string deleteStaff = "DELETE FROM staff WHERE staff_ID = '" + staffID + "'";
					const char* ds = deleteStaff.c_str();
					qstate = mysql_query(conn, ds);

					if (qstate)
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

					string deleteUA = "DELETE FROM useraccount WHERE user_account_ID = '" + userID + "'";
					const char* dUA = deleteUA.c_str();
					qstate = mysql_query(conn, dUA);

					if (qstate)
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
					else
						cout << "\tStaff remomved.\n";
				}
				else if (r == 'n' || r == 'N')
				{
					cout << "\tOperation cancelled.\n";
					break;
				}
				else
					cout << "\tInvalid input. Try again.\n";
			} while (r != 'y' && r != 'Y' && r != 'n' && r != 'N');

			do {
				cout << "\n\tContinue? (Y/N): ";
				cin >> p;
				cin.ignore(100, '\n');
				if (p == 'n' || p == 'N')
				{
					cout << "\n\tPress enter to back to Main Menu...";
					_getch();
					system("cls");
					AdminMainMenu();
				}
				else if (p == 'y' || p == 'Y')
					continue;
				else
					cout << "\tInvalid input.\n";
			} while (p != 'y' && p != 'Y' && p != 'n' && p != 'N');
		}
		else
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	} while (p == 'y' || p == 'Y');
}

void CustomerList()
{
	system("cls");
	header();
	cout << "\t-------------------------------------------------------------------------------------------" << endl;
	cout << "\t|				        CUSTOMER LIST				          |" << endl;
	cout << "\t-------------------------------------------------------------------------------------------" << endl << endl;

	qstate = mysql_query(conn, "SELECT * FROM customer");
	if (!qstate)
	{
		cout <<"\t" << left << setw(15) << "Customer ID" << setw(35) << "Customer Name" << setw(20) << "Customer Tel." << setw(15) << "Customer Debt Amount" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			cout << "\t" << left << setw(15) << row[0] << setw(35) << row[1] << setw(20) << row[2] << setw(15) << row[8] << endl;
	}
	else
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;


	char txt;
	do {
		cout << "\n\n\tDo you want to export as text file?(Y/N): ";
		cin >> txt;
		cin.ignore(100, '\n');

		if (txt == 'Y' || txt == 'y') {
			CreateDirectory(L"C:\\Beverage Warehouse Management System\\Customer List", NULL);
			string filename = "C:\\Beverage Warehouse Management System\\Customer List\\Customer List " + today_date + ".txt";
			ofstream writeFile(filename, ios::out);
			auto cout_buff = cout.rdbuf();	// pointer to cout buffer
			cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

			cout << setw(50) << right << "CUSTOMER LIST" << endl << endl;

			qstate = mysql_query(conn, "SELECT * FROM customer");
			if (!qstate)
			{
				cout << left << setw(15) << "Customer ID" << setw(35) << "Customer Name" << setw(20) << "Customer Tel." << setw(15) << "Customer Debt Amount" << endl;

				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
					cout << left << setw(15) << row[0] << setw(35) << row[1] << setw(20) << row[2] << setw(15) << row[8] << endl;
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			cout.rdbuf(cout_buff);	// go back to cout buffer
			cout << "\tSuccessfully exported to ";
			cout << filename << endl;
		}
		else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
			cout << "\tInvalid input.Try again.";
	} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');

	cout << "\n\tPress enter to back to Main Menu...";
	_getch();
	system("cls");
	AdminMainMenu();
}

void SalesReport()
{
	system("cls");
	header();
	cout << "\t-----------------------------------------------------" << endl;
	cout << "\t|		    SALES REPORT		    |" << endl;
	cout << "\t-----------------------------------------------------" << endl << endl;

	char sr;
	string date;
	string productID[1000], quantity[1000], subtotal[1000];
	string g_productID[1000];
	int g_quantity[1000] = { 0 };

	cout << setw(15) << "\t1 - By Date\n";
	cout << setw(16) << "\t2 - By Month\n";
	cout << setw(15) << "\t3 - By Year\n";

	do {
		cout << "\n\tEnter your choice: ";
		cin >> sr;
		cin.ignore(1000, '\n');

		if (sr == '1')
			cout << "\tEnter the date(YYYY-MM-DD): ";
		else if (sr == '2')
			cout << "\tEnter the year and month(YYYY-MM): ";
		else if (sr == '3')
			cout << "\tEnter the year(YYYY): ";
		else
			cout << "\tInvalid input. Try again.";

	} while (sr != '1' && sr != '2' && sr != '3');

	//cin.ignore(1, '\n');
	getline(cin, date);

	string checkPI_query = "SELECT * FROM orderdetails WHERE order_ID IN (SELECT order_ID FROM orders WHERE order_date LIKE '%" + date + "%')";
	const char* checkPI = checkPI_query.c_str();
	qstate = mysql_query(conn, checkPI);
	if (!qstate)
	{
		int i = 0;
		res = mysql_store_result(conn);

		if (res->row_count == 0) {
			cout << "\tSales report for this date is not available.\n";
			cout << "\n\tPress enter to back to Main Menu...";
			_getch();
			system("cls");
			AdminMainMenu();
		}
		else
		{
			while (row = mysql_fetch_row(res))
			{
				productID[i] = row[2];
				quantity[i] = row[3];
				i++;
			}

			int a = 0;
			for (int j = 0; j < i + 1; j++)
			{
				if (!productID[j].empty()) {
					g_productID[a] = productID[j];
					g_quantity[a] = stoi(quantity[j]);

					for (int k = j + 1; k < i + 1; k++)
					{
						if (g_productID[a] == productID[k]) {
							g_quantity[a] = g_quantity[a] + stoi(quantity[k]);
							productID[k].clear();
						}
					}
					productID[j].clear();
					a++;
				}
			}

			string quantity = "0";
			system("cls");
			header();
			cout << "\t----------------------------------------------------------------------------" << endl << endl;
			cout << "\t\t\t\t\tSALES REPORT " << endl;
			cout << "\t\t\t\t\t " << date << endl << endl;
			cout << "\t" << left << setw(15) << "Product ID" << setw(45) << "Product Name" << setw(20) << "Quantity Sold" << endl;
			string checkPN_query = "SELECT * FROM product";
			const char* checkPN = checkPN_query.c_str();
			qstate = mysql_query(conn, checkPN);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					for (int b = 0; b <= a; b++)
					{
						if (row[0] == g_productID[b])
							quantity = to_string(g_quantity[b]);
					}

					cout << "\t" << left << setw(15) << row[0] << setw(45) << row[1] << setw(20) << quantity << endl;

					quantity = "0";
				}
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

			string amount;
			double total = 0;
			string checkTE_query = "SELECT * FROM orders WHERE order_date LIKE '%" + date + "%'";
			const char* checkTE = checkTE_query.c_str();
			qstate = mysql_query(conn, checkTE);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					amount = row[4];
					total += stod(amount);
				}
			}
			else
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			cout << "\n\tTotal Aamount Earned for " << date << " = RM " << fixed << setprecision(2) << total << endl;

			int temp;
			string temp2;
			for (int c = 0; c < a; ++c)
			{
				for (int d = c + 1; d < a; ++d)
				{
					if (g_quantity[c] < g_quantity[d])
					{
						temp = g_quantity[c];
						g_quantity[c] = g_quantity[d];
						g_quantity[d] = temp;

						temp2 = g_productID[c];
						g_productID[c] = g_productID[d];
						g_productID[d] = temp2;
					}
				}
			}

			cout << "\n\tTOP 5 SELLING BEVERAGES" << endl;
			for (int e = 0; e < 5; e++)
			{
				string checkT5_query = "SELECT * FROM product WHERE product_ID = '" + g_productID[e] + "'";
				const char* checkT5 = checkT5_query.c_str();
				qstate = mysql_query(conn, checkT5);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					while (row = mysql_fetch_row(res))
						cout << "\t" << e + 1 << ". " << row[1] << endl;
				}
				else
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			}

			char txt;
			do {
				cout << "\n\n\tDo you want to export as text file?(Y/N): ";
				cin >> txt;
				cin.ignore(100, '\n');

				if (txt == 'Y' || txt == 'y') {
					CreateDirectory(L"C:\\Beverage Warehouse Management System\\Sales Report", NULL);
					string filename = "C:\\Beverage Warehouse Management System\\Sales Report\\Sales Report " + date + ".txt";
					ofstream writeFile(filename, ios::out);
					auto cout_buff = cout.rdbuf();	// pointer to cout buffer
					cout.rdbuf(writeFile.rdbuf());	// substitute cout buffer with local ofstream buffer

					cout << "\t\t\t\tSALES REPORT " << endl;
					cout << "\t\t\t\t " << date << endl << endl;
					cout << left << setw(15) << "Product ID" << setw(45) << "Product Name" << setw(20) << "Quantity Sold" << endl;
					string checkPN_query = "SELECT * FROM product";
					const char* checkPN = checkPN_query.c_str();
					qstate = mysql_query(conn, checkPN);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							for (int b = 0; b <= a; b++)
							{
								if (row[0] == g_productID[b])
									quantity = to_string(g_quantity[b]);
							}

							cout << left << setw(15) << row[0] << setw(45) << row[1] << setw(20) << quantity << endl;

							quantity = "0";
						}
					}
					else
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

					string amount;
					double total = 0;
					string checkTE_query = "SELECT * FROM orders WHERE order_date LIKE '%" + date + "%'";
					const char* checkTE = checkTE_query.c_str();
					qstate = mysql_query(conn, checkTE);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							amount = row[4];
							total += stod(amount);
						}
					}
					else
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
					cout << "\nTotal Aamount Earned for " << date << " = RM " << fixed << setprecision(2) << total << endl;

					int temp;
					string temp2;
					for (int c = 0; c < a; ++c)
					{
						for (int d = c + 1; d < a; ++d)
						{
							if (g_quantity[c] < g_quantity[d])
							{
								temp = g_quantity[c];
								g_quantity[c] = g_quantity[d];
								g_quantity[d] = temp;

								temp2 = g_productID[c];
								g_productID[c] = g_productID[d];
								g_productID[d] = temp2;
							}
						}
					}

					cout << "\nTOP 5 SELLING BEVERAGES" << endl;
					for (int e = 0; e < 5; e++)
					{
						string checkT5_query = "SELECT * FROM product WHERE product_ID = '" + g_productID[e] + "'";
						const char* checkT5 = checkT5_query.c_str();
						qstate = mysql_query(conn, checkT5);
						if (!qstate)
						{
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
								cout << e + 1 << ". " << row[1] << endl;
						}
						else
							cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
					}
					cout.rdbuf(cout_buff);	// go back to cout buffer
					cout << "\tSuccessfully exported to ";
					cout << filename << endl;
				}
				else if (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n')
					cout << "\tInvalid input.Try again.";

			} while (txt != 'Y' && txt != 'y' && txt != 'N' && txt != 'n');
		}
	}
	else
	cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	cout << "\n\tPress enter to back to Main Menu...";
	_getch();
	system("cls");
	AdminMainMenu();
}

int main() {
	db_response::ConnectionFunction();
	today_date = getTodayDate();
	//now_time = getNowTime();
	checkOverdue();
	CreateDirectory(L"C:\\Beverage Warehouse Management System", NULL);	//create a directory if does not exist
	WelcomePage();
	return 0;
}