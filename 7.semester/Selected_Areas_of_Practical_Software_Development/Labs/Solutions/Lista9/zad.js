const mode = 'async';

const { Client } = require('pg')

const client = new Client({
    user: 'postgres',
    host: 'localhost',
    database: 'weppo_db',
    password: '',
    port: 5432,
});

client.connect();

createquery = "CREATE TABLE OSOBA (\
	pid SERIAL PRIMARY KEY,\
	name varchar(40),\
	surname varchar(40),\
	sex char(1),\
	age integer\
	CONSTRAINT con1 CHECK (age > 0 AND age < 120)\
	CONSTRAINT con2 CHECK (sex='f' OR sex='m' OR sex='F' OR sex='M'))"

insertquery = "INSERT INTO osoba VALUES (default, 'Anna', 'Kowalska', 'f', '32') RETURNING *;\
            INSERT INTO osoba VALUES (default, 'Igor', 'Tryhub', 'm', '21') RETURNING *;\
            INSERT INTO osoba VALUES (default, 'Wiktor', 'Zychla', 'm', '31') RETURNING *;\
            INSERT INTO osoba VALUES (default, 'Jan', 'Malinowski', 'm', '52') RETURNING *;\
            INSERT INTO osoba VALUES (default, 'Piotr', 'Kowalczyk', 'M', '32') RETURNING *;\
            INSERT INTO osoba VALUES (default, 'Maria', 'Morawska', 'f', '19') RETURNING *";

selectquery = "SELECT * FROM OSOBA";

updatequery = "UPDATE osoba\
    SET name = 'Janusz'\
    WHERE name = 'Jan'";

deletequery = "DELETE FROM osoba\
    WHERE name='Anna'";

many2manyquery = "CREATE TABLE MIEJSCE_PRACY (\
	cid SERIAL PRIMARY KEY,\
	company varchar(40) NOT NULL,\
    city varchar(40));\
    CREATE TABLE OSOBA_MIEJSCE_PRACY (\
        id_osoba integer REFERENCES osoba (pid),\
        id_miejsce_pracy integer REFERENCES miejsce_pracy (cid));\
        INSERT INTO miejsce_pracy VALUES (default, 'Real', 'Madrid');\
    INSERT INTO miejsce_pracy VALUES (default, 'UWr', 'Wroclaw');\
    INSERT INTO miejsce_pracy VALUES (default, 'ABC', 'London');\
    INSERT INTO miejsce_pracy VALUES (default, 'Zabka', 'Czestochowa');\
    INSERT INTO osoba_miejsce_pracy VALUES (\
        (SELECT pid FROM osoba\
            WHERE surname='Tryhub'),\
        (SELECT cid FROM miejsce_pracy\
            WHERE company='UWr')\
    );\
    INSERT INTO osoba_miejsce_pracy VALUES (\
        (SELECT pid FROM osoba\
            WHERE surname='Zychla'),\
        (SELECT cid FROM miejsce_pracy\
            WHERE company='UWr')\
    );\
    INSERT INTO osoba_miejsce_pracy VALUES (\
        (SELECT pid FROM osoba\
            WHERE surname='Tryhub'),\
        (SELECT cid FROM miejsce_pracy\
            WHERE company='Zabka')\
    );\
    SELECT * FROM osoba_miejsce_pracy";

// const querylist = [createquery, insertquery, selectquery, updatequery, selectquery, deletequery, deletequery];
var query = many2manyquery;

// for(q in querylist){

if (mode=='promise'){
    client.query(query)
    .then(res => {
        if(query==insertquery){
            lastid = res.rows[0]['pid'];
            console.log('Last ID: ' + lastid);
        }
        console.log("Success!");
        console.log(res.rows);
        client.end();
    })
    .catch(e => {
        console.error(e.stack);
        client.end();
    })
}
else if (mode=='async') {
    async function doquery(){
        try {
            const res = await client.query(query);
            if(query==insertquery){
                lastid = res.rows[0]['pid'];
                console.log('Last ID: ' + lastid);
            }
            console.log("Success!");
            console.log(res.rows);
        } catch(err) {
            console.log(err.stack);
        }
        client.end();
    };   
    doquery();
}
// }
// client.end();
