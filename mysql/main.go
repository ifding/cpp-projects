package main

import (
    "database/sql"
    "fmt"
    "log"

    _ "github.com/go-sql-driver/mysql"
)

// Reference: 
// http://go-database-sql.org/index.html

type City struct {
    Id         int
    Name       string
    Population int
}

func main() {
    db, err := sql.Open("mysql", "user:password@/dbname")
    defer db.Close()

    if err != nil {
        log.Fatal(err)
    }

    //sql := "INSERT INTO cities(name, population) VALUES ('Moscow', 12506000)"
    sql := "DELETE FROM cities WHERE id IN (2, 4, 9, 13)"

    exe, err := db.Exec(sql)
    if err != nil {
        log.Fatal(err)
    }


    //stmt, err := db.Prepare("SELECT * FROM cities WHERE id = ?")
    //stmt, err := db.Prepare("SELECT name FROM cities WHERE id = ?")
    stmt, err := db.Prepare("INSERT INTO cities(name, population) VALUES(?, ?)")
    defer stmt.Close()

    if err != nil {
        log.Fatal(err)
    } 

    exe, err = stmt.Exec("Moscow", 12506000)
    if err != nil {
        log.Fatal(err)
    }
    lastId, err := exe.LastInsertId()
    if err != nil {
        log.Fatal(err)
    }
    fmt.Println(lastId)  


    res, err := db.Query("SELECT * FROM cities")
    defer res.Close()

    for res.Next() {
        var city City
        err := res.Scan(&city.Id, &city.Name, &city.Population)

        if err != nil {
            log.Fatal(err)
        }
        fmt.Printf("%v\n", city)
    }
}