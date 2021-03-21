#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite/sqlite3.h"
#include <string> 
#include "cppsqlite2.h"

using namespace std;
sqlite3* db;
char* zErrMsg = 0;
int rc;
string sql;
int64_t IDs = 0;
string names;
string emails;
int64_t stationIds;
string carBrands;
int64_t speeds;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
static int open_database() {
    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stdout, "Opened database successfully\n");
    }
    return 0;
}
static int create_table() {
    open_database();
    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS CAM(" \
        "ID INT PRIMARY KEY    NOT NULL ," \
        "name TEXT," \
        "stationId INT   NOT NULL," \
        "carBrand TEXT," \
        "email TEXT," \
        "speed INT   NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "CAM Table created successfully\n");
    }
    /* Open database */

    sql = "CREATE TABLE IF NOT EXISTS AUDIT(" \
        "_ID INT   NOT NULL ," \
        "ENTRY_DATE TEXT   NOT NULL);";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "AUDIT Table created successfully\n");
    }
    sql = "CREATE TABLE IF NOT EXISTS lead_logs(" \
        "old_stationId INT   NOT NULL ," \
        "old_speed INT   NOT NULL);";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "lead_logs Table created successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int insert_into(Person* ptr1) {
    Person* ptr = ptr1;
    int64_t veriSayisi;
    open_database();
    /* Create SQL statement */
    cout << "Gireceginiz veri sayisini girin: " << endl;
    cin >> veriSayisi;
    for (int i = 0; i < veriSayisi; ++i) {
        cout << "ID: ";
        cin >> ptr->ID;
        cin.ignore();
        IDs = ptr->ID;
        cout << "Name: ";
        getline(cin, ptr->name);
        names = ptr->name;
        cout << "Email: ";
        getline(cin, ptr->email);
        emails = ptr->email;
        cout << "Station ID: ";
        cin >> ptr->Car->stationId;
        cin.ignore();
        stationIds = ptr->Car->stationId;
        cout << "Car Brand: ";
        getline(cin, ptr->Car->carBrand);
        carBrands = ptr->Car->carBrand;
        cout << "Speed: ";
        cin >> ptr->Car->speed;
        speeds = ptr->Car->speed;
        cout << endl;
        sql = "INSERT INTO CAM (ID,name,stationId,carBrand,email,speed) "  \
            "VALUES (" + to_string(IDs) + ",'" + names + "'," + to_string(stationIds) + ",'" + carBrands + "','" + emails + "'," + to_string(speeds) + " );";
        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else {
            fprintf(stdout, "Records created successfully\n");
        }
    }
    sqlite3_close(db);
    return 0;
}
static int select_(const char* data) {
    open_database();
    sql = "SELECT * from CAM";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Select operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int update_(const char* data1) {
    const char* data = data1;
    select_(data);
    open_database();

    int64_t up_ID;
    cout << "Hizina update atmak istediginiz ID degerini giriniz : " << endl;
    cin >> up_ID;
    sql = "UPDATE CAM set speed = 69 where ID="+to_string(up_ID)+"; " \
        "SELECT * from CAM";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Update operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}

static int delete_(const char* data1) {
    const char* data = data1;
    select_(data);
    open_database();
    
    int64_t del_ID;
    cout << "Silmek istediginiz verinin ID degerini giriniz : " << endl;
    cin >> del_ID;
    sql = "DELETE from CAM where ID="+to_string(del_ID)+"; " \
        "SELECT * from CAM";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Delete operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int create_view() {
    open_database();
    sql = "CREATE VIEW IF NOT EXISTS details_view AS SELECT ID, name, stationId, carBrand, email, speed FROM CAM ORDER BY ID, name;";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "details_view VIEW created successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int create_triggers(const char* data){
    open_database();
    sql = "CREATE TRIGGER IF NOT EXISTS audit_log AFTER INSERT ON CAM BEGIN INSERT INTO AUDIT(_ID, ENTRY_DATE) VALUES (new.ID, datetime('now'));END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "First Trigger Records created successfully\n");
    }
    sql = "CREATE TRIGGER IF NOT EXISTS log_contact_after_update AFTER UPDATE ON CAM WHEN old.stationId <> new.stationId OR old.speed <> new.speed BEGIN INSERT INTO lead_logs( old_stationId, old_speed) VALUES (old.stationId,old.speed);END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Second Trigger Records created successfully\n");
    }
    sql = "CREATE TRIGGER IF NOT EXISTS validate_email_before_insert_leads  BEFORE INSERT ON CAM BEGIN SELECT CASE WHEN NEW.email NOT LIKE '%_@__%.__%' THEN RAISE (ABORT,'Invalid email address')END;END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Third Trigger Records created successfully\n");
    }
    sql = "CREATE TRIGGER IF NOT EXISTS view_delete_trig INSTEAD OF delete ON details_view BEGIN SELECT RAISE (ABORT,'View data is indelible');END; ";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Fourth Trigger Records created successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int select_triggers_on_CAM(const char* data) {
    /* Create merged SQL statement */
    open_database();
    sql = " SELECT name FROM sqlite_master WHERE type = 'trigger' AND tbl_name = 'CAM'; ";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Trigger select operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
static int drop_trigger(const char* data) {
    open_database();
    sql = "DROP TRIGGER IF EXISTS audit_log";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Trigger drop operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
int main()
{
    const char* data = "Callback function called";
    Person person;
    VehicleInfo vehicleinfo;
    person.Car = &vehicleinfo;
    Person* ptr = &person;
    create_table();
    insert_into(ptr);
    update_(data);
    delete_(data);
    select_(data);
    create_view();
    create_triggers(data);
    select_triggers_on_CAM(data);
    drop_trigger(data);
    
    return 0;
}
