#include <iostream>
#include<winsock.h>
#include<windows.h>
#include<sstream>
#include<ctime>
#include<string>
#include<mysql.h>
#include<cstring>
#include<conio.h>
#include<cstring>
#include<map>
using namespace std;


int users =0;
int count_attempt=0;
int curr_user=0;
int user_no=0;
map <string,string> issued;
string loggedin_user="";

void showtime()
{
	time_t now = time(0);
	char *dt = ctime(&now);
	cout<<" \t \t \t \t \t " << dt;
	cout<<endl<<endl;
}

class user
{
private:
string username;
string password;
public:
void adduser()
{
	MYSQL *conn;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
	if(conn)
        cout<<"connected"<<endl;
    else cout<<"not connected"<<endl;

	cout<<"Username:";
	cin>>username;
	cout<<"Password:";
	cin>>password;

	int qstate =0;
	stringstream ss;
	ss<<"INSERT INTO users(username,password) VALUES('"<<username<<"','"<<password<<"')";
	string query =  ss.str();
        const char* q= query.c_str();
        qstate = mysql_query(conn,q);
        if(qstate==0)
            cout<<" \t \t \t \t \t Record Inserted Successfully"<<endl;
        else cout<<"Failed"<<endl;

	users++;
}

// This method is protected for admin use only
void no_users()
{

   if(loggedin_user!="admin")
    {
        cout<<" \t \t \t \t \t Not Authorized. Only admin can view all users details"<<endl;
        return ;
    }

	cout<<"The users are:  "<<endl;
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
	if(conn)
    {
        int qstate = mysql_query(conn,"SELECT username FROM users");
    //    bool flag=false;
        if(!qstate)
        {
            res = mysql_store_result(conn);
            int i=1;
            while(row = mysql_fetch_row(res))
                {
                    cout<<i<<"."<<row[0]<<endl;
                    i++;
                }
        }
    }


}

// This method is used to take user input and check against the data in the database to login to the system
void login()
{
	if(curr_user==1)
	{
	cout<<" \t \t \t \t \t Already logged in";
	return;
	}
	if(count_attempt==3)
	{
		cout<<endl<<endl;
		cout<<" \t \t \t \t \t No more attempts";
		exit(0);
	}
	cout<<"Enter Username and Password to Login"<<endl;
	string user_name;
	string pass_word;

	cout<<"Username:";
	cin>>user_name;
	cout<<"Password:";
	cin>>pass_word;
	cin.ignore();
	system("cls");


	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
	if(conn)
    {
        int qstate = mysql_query(conn,"SELECT username,password FROM users");
        bool flag=false;
        if(!qstate)
        {
            res = mysql_store_result(conn);
            while(row = mysql_fetch_row(res))
                {
                if(row[0]==user_name && row[1]==pass_word)
                {
                        loggedin_user = row[0];
                        cout<<" \t \t \t \t \t \t Login Successful"<<endl;
                        cout<<" \t \t \t \t \t \t WELCOME "<< loggedin_user<<endl;
                        curr_user=1;
                        flag=true;
                        break;
                }
                }
                if(!flag)
                {
                    cout<<" \t \t \t \t \t \t Incorrect Username or Password"<<endl;
                    cout<<" \t \t \t \t \t \t Press Enter and Try again"<<endl;
                    cin.ignore();
                    system("cls");
                    count_attempt++;
                    login();
                }


        }
        else
        {
            cout<<" \t \t \t \t \t No Accounts Registered";
        }

    }
}

	void logout()
	{
		curr_user=0;
	}


};
class book : public user
{
	long long int bno;
	string bookname;
	string aname;
	string default_issed = "None";
	public:
	    // This method is protected for admin use only
		void addbook()
		{
			if(loggedin_user=="")
			{
				cout<<" \t \t \t \t \t Please Login"<<endl;
				return ;
			}
            if(loggedin_user!="admin")
			{
				cout<<" \t \t \t \t \t Not Authorized. Only admin can add new books"<<endl;
				return ;
			}

			cout<<"Enter book number \n";
			cin>>bno;
            cin.ignore();
			cout<<"Enter bookname: \n";
			cin>>bookname;
            cin.ignore();
			cout<<"Enter author name: \n";
			cin>>aname;
            string nul="";
			MYSQL* conn;
			conn = mysql_init(0);
			conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
			stringstream ss;
			ss<<"INSERT INTO book(BookNo,Book_Name,Author,Issued_By) VALUES('"<<bno<<"','"<<bookname<<"','"<<aname<<"','"<<default_issed<<"')";
			int qstate=0;
			string query =  ss.str();
            const char* q= query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate==0)
            {
            cout<<" \t \t \t \t \t Book Added Successfully!"<<endl;
            return;
            }
            else
            {
                cout<<" \t \t \t \t \t Failed"<<endl;
                return;
            }

		}
        
        // This method is used to search for a particular book in the database
		bool searchbook(string no)
		{
			if(loggedin_user=="")
			{
				 cout<<" \t \t \t \t \t Please Login"<<endl;
				 return false;
			}

			bool flag=false;
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo,Book_Name,Author from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(row[0]== no)
                        {
                            cout<<"Book Details are"<<endl;
                            cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<endl;
                            flag=true;
                            return true;
                        }
                }
            }
            else cout<<"Failed"<<endl;
            if(!flag)
                {
                    cout<<"Book Not Found"<<endl;
                    return false;
                }
            }
		}
		
        // This method is used to view all the books in the database
		void allbooks()
		{
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo,Book_Name,Author,Issued_By from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        cout<<" \t \t \t \t \t "<<row[0] <<"(No)"<<" "<<row[1]<<"(Name)"<<"  "<<row[2]<<"(Author)"<<" "<<row[3]<<"(Issued by)"<<endl;
                }

            }
            }
            else cout<<"Failed"<<endl;
		}

    // This method is used to update the issue column of book table in database
	void issue()
	{
		if(loggedin_user=="")
		{
			cout<<"Please Login"<<endl;
			return ;
		}
		string bno;
		cout<<" \t Enter book number to be issued"<<endl;
		cin>>bno;
		bool found = searchbook(bno);
		if(found)
		{
            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "library" ,0,NULL,0);
            stringstream ss;
            int qstate=0;
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo, Book_Name, Author, Issued_By from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(bno==row[0])
                        {
                            ss<<"UPDATE book SET Issued_By=('"<<loggedin_user<<"')";
                            string query =  ss.str();
                              const char* q= query.c_str();
                              qstate = mysql_query(conn,q);
                              if(qstate==0){
                                cout<<" \t \t \t \t \t Book Issued Successfully "<<endl;
                                cout<<" \t \t \t Please Return the book within 15 days otherwise fine of Rs 150 will be applied"<<endl;
                              }
                              else cout<<"Failed"<<endl;
                              issued.insert({loggedin_user, bno});
                              return;
                        }
                }

            }
		}
    }
	else cout<<" \t \t \t \t \t Book Not Found"<<endl;

}
    // This method is used to return an issued book 
	void deposit()
	{
		map<string ,string>::iterator itr;
		for(itr=issued.begin();itr!=issued.end();itr++)
		{
			if(itr->first==loggedin_user)
			{
			issued.erase(loggedin_user);
			cout<<" \t \t \t \t \t Book Returned Successfully";
			return ;
			}
		}
	}
    
    // This method is used to print the current user details
	void print_user()
	{
		 map<string ,string>:: iterator itr;
		 bool flag=false;
				 for(itr=issued.begin();itr!=issued.end();itr++)
				 {
				 	if(itr->first==loggedin_user )
				 	{
					 cout<<" \t \t \t \t \t Current user "<<itr->first<<"   has issued book no:   "<<itr->second;
					 flag=true;
					 }

				}
				if(!flag)
					cout<<" \t \t \t \t \t "<<loggedin_user<<" has issued no book";
				 	cout<<endl;
	}

};


int main()
{
  showtime();
	cout<<" \t \t \t \t \t Library Management System "<<endl<<endl<<endl<<endl;
	user u1;
	int x;
	int choice=0;
	book b1;
	do
	{
		cout<<endl;
		cout<<"Menu:"<<endl;
		cout<<"1. Login"<<endl;
		cout<<"2. Register"<<endl;
		cout<<"3. All Books"<<endl;
		cout<<"4. Search Book"<<endl;
		cout<<"5. Issue Book"<<endl;
		cout<<"6. Return Book"<<endl;
		cout<<"7. Get all users"<<endl;
		cout<<"8. Add Book"<<endl;
		cout<<"9. Current user details"<<endl;
		cout<<"10. Logout"<<endl;
		cout<<"0 to exit"<<endl;
		cin>>x;
	string no2;
	switch(x)
	{
		case 1 : u1.login();
				 break;
		case 2 : u1.adduser();
				 u1.login();
				 break;
		case 3 : b1.allbooks();
				 break;
		case 4 : cout<<"Enter book no";
				 cin>>no2;
				 b1.searchbook(no2);
		case 5 : b1.issue();
				 break;
		case 6 : b1.deposit();
				 break;
		case 7 : u1.no_users();
				 break;
				 break;
		case 8 :  b1.addbook();
				 break;
		case 9 : b1.print_user();
				 break;
		case 10:  u1.logout();
				 break;
		case 0 : choice=1;

	}
	}
	while(choice==0);

}