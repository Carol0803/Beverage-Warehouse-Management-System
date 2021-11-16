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
string user_account_ID;

//User Management
void WelcomePage();
void Login();
void SignUp();
//Customer's Part
void CustomerMainMenu();
void PlaceOrder();
void Invoice();

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

void WelcomePage(){
	int selection;
	do {
		system("cls");
		cout << "\nBeverage Warehouse Management System" << endl << endl;
		cout << "1 - Log In\n2 - Sign Up\n0 - Exit\n\n";
		cout << "Enter your selection: ";
		cin >> selection;

		if (selection == 1) {
			Login();
		}
		else if (selection == 2) {
			SignUp();
		}
		else if (selection == 0) {
			cout << "\nExiting...\nProgram Terminated.\n";
			break;
		}
		else {
			cout << "Invalid selection.Please try again.";
		}
	} while (selection != 0 && selection != 1 && selection != 2);
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
				
				system("cls");
				cout << setw(27) << "Hi, " << username << "!" << endl;
				cout << "Please select an option by entering the number labelled." << endl;
			
				if (access_type == "A")
					cout << "Administrator";
				else if (access_type == "S")
					cout << "Staff";
				else
					CustomerMainMenu();
			}
		}
		else
		{
			char c;
			cout << "\n\nWrong User ID or Password. \nDo you want to try again? (Y/N): ";
			do {
			cin >> c;
				if (c == 'y' || c == 'Y')
					Login();
				else if (c == 'n' || c == 'N')
					WelcomePage();
				else
				{
					cout << "Invalid input." << endl;
					cout << "Please enter Y for yes or N for no: ";
				}
			} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
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
	int selection;
	do {
		cout << endl << setw(13) << "MAIN MENU" << endl << endl;
		cout << "1 - Place order\n2 - View Invoice\n0 - Log out\n\n";
		cout << "Enter your selection: ";
		cin >> selection;

		if (selection == 1) {
			PlaceOrder();
		}
		else if (selection == 2) {
			Invoice();
		}
		else if (selection == 0) {
			cout << "\nLogging out...\n";
			WelcomePage();
		}
		else {
			cout << "Invalid selection.Please try again.";
		}
	} while (selection != 0 && selection != 1 && selection != 2);
}

void PlaceOrder() 
{
	system("cls");
	cout << setw(36) << "PLACE ORDER" << endl << endl;

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
	
	char c;
	cout << "Do you want to order a product? (Y/N): ";
	do {
		cin >> c;
		if (c == 'y' || c == 'Y')
		{
			string custID;
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

			#pragma warning(disable : 4996)
			time_t today = time(0);
			tm* ltm = localtime(&today);
			string today_date = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
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

			string productID;
			int quantity;
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

					} while (quantity > stoi(current_quantity));
				}
				else
				{
					cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
				}
			}
			else
			{
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			}
		}
		else
		{
			cout << "Invalid input." << endl;
			cout << "Please enter Y for yes or N for no: ";
		}
	} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
}

void Invoice() {}

int main() {
	system("cls");          
	system("title My Project");
	db_response::ConnectionFunction();
	WelcomePage();
	return 0;
}