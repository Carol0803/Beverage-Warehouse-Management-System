CREATE TABLE USERACCOUNT (
	user_account_ID	INT(8)			ZEROFILL   NOT NULL   AUTO_INCREMENT   UNIQUE,
	username		VARCHAR(50)		NOT NULL,
	password		VARCHAR(20)		NOT NULL,
	access_type		CHAR(1)			NOT NULL,
	PRIMARY KEY(user_account_ID)
);

CREATE TABLE STAFF (
	staff_ID			INT(5)			ZEROFILL   NOT NULL   AUTO_INCREMENT   UNIQUE,
	staff_name			VARCHAR(50)		NOT NULL,
	staff_tel			CHAR(12)		NOT NULL,
	staff_address1		VARCHAR(50)		NOT NULL,
	staff_address2		VARCHAR(50),
	staff_area			VARCHAR(20)		NOT NULL,
	staff_postcode		CHAR(5)			NOT NULL,
	staff_state			VARCHAR(20)		NOT NULL,
	user_account_ID		INT(8)			ZEROFILL   NOT NULL		UNIQUE,
	PRIMARY KEY(staff_ID),
	FOREIGN KEY(user_account_ID)REFERENCES USERACCOUNT(user_account_ID)
);

CREATE TABLE PRODUCT (
	product_ID			INT(5)			ZEROFILL   NOT NULL   AUTO_INCREMENT   UNIQUE,
	product_name		VARCHAR(50)		NOT NULL,
	price_per_unit		NUMERIC(6,2)	NOT NULL,
	current_quantity	NUMERIC(10)		NOT NULL,
	reorder_level		NUMERIC(10)		NOT NULL,
	PRIMARY KEY(product_ID)
);

CREATE TABLE ORDERS (
    order_ID			INT(10)			NOT NULL   AUTO_INCREMENT   UNIQUE,
	order_date			DATE			NOT NULL,
	delivery_status		CHAR(1)			NOT NULL,
	total_quantity		NUMERIC(10)	,
	total_amount		NUMERIC(9,2),
	cust_ID				INT(7)			ZEROFILL   NOT NULL		UNIQUE,
	order_details_ID	INT(10),
	payment_details_ID	INT(10),
	PRIMARY KEY(order_ID),
	FOREIGN KEY(order_details_ID)REFERENCES ORDERDETAILS(order_details_ID),
	FOREIGN KEY(payment_details_ID)REFERENCES PAYMENTDETAILS(payment_details_ID)
)AUTO_INCREMENT=1010000000;

CREATE TABLE PAYMENTDETAILS (
	payment_details_ID	INT(10)			NOT NULL   AUTO_INCREMENT   UNIQUE,
	payment_date		DATE,
	payment_status		CHAR(1)			NOT NULL,
	payment_method		CHAR(1)			NOT NULL,
	amount_paid			NUMERIC(9,2)	DEFAULT 0.00,
	payment_duedate		DATE			NOT NULL,
	order_ID			INT(10)			NOT NULL	UNIQUE,
	PRIMARY KEY(payment_details_ID),
	FOREIGN KEY(order_ID)REFERENCES ORDERS(order_ID)
)AUTO_INCREMENT=1011000000;

CREATE TABLE ORDERDETAILS (
	order_details_ID	INT(10)		NOT NULL   AUTO_INCREMENT   UNIQUE,
	order_ID			INT(10)		NOT NULL,
	product_ID			INT(5)		ZEROFILL   NOT NULL,
	quantity			NUMERIC(10)		NOT NULL,
	sub_total			NUMERIC(9,2)	NOT NULL,
	PRIMARY KEY(order_details_ID),
	FOREIGN KEY(order_ID)REFERENCES ORDERS(order_ID),
	FOREIGN KEY(product_ID)REFERENCES PRODUCT(product_ID)
)AUTO_INCREMENT=1012000000;

CREATE TABLE CUSTOMER (
	cust_ID				INT(7)			ZEROFILL   NOT NULL   AUTO_INCREMENT   UNIQUE,
	cust_name			VARCHAR(50)		NOT NULL,
	cust_tel			CHAR(12)		NOT NULL,
	cust_address1		VARCHAR(50)		NOT NULL,
	cust_address2		VARCHAR(50),
	cust_area			VARCHAR(20)		NOT NULL,
	cust_postcode		CHAR(5)			NOT NULL,
	cust_state			VARCHAR(20)		NOT NULL,
	cust_debt_amount	NUMERIC(9,2)	DEFAULT 0.00,
	user_account_ID		INT(8)			ZEROFILL   NOT NULL	UNIQUE,
	order_ID			INT(10)			UNIQUE,
	PRIMARY KEY(cust_ID),
	FOREIGN KEY(user_account_ID)REFERENCES USERACCOUNT(user_account_ID),
	FOREIGN KEY(order_ID)REFERENCES ORDERS(order_ID)
);