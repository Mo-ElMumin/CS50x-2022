/* selecting data from a table */
Select
    *  /* returns the all data from tabl e */
    column_1_name, cloumn_2_name /* retrives data from that specific column, where named without quotes */
    column_1_name as new_column_name /* lets you use an alias, to change name of column in the returned results */
    column_1_name as "new column name" /* double quotes let you include spaces in new column name */

 /* let you decide which table to get the data from */
 From
    "Table 1 name", "table 2 name" /* only retrives data from the tables named, without quotes */
    /* lets you select rows where a specific column has a specific value */
    where column = specific_value

/* example */
select id from people where age = 50
/* retrices the id of people who are aged 50 years old */
/* return the id column data, from the people table, if the value of the age column is equal to 100 */

/* other comparisons */
where age > 50 /* greater than a value */
where age < 50 /* Less than a value */
where age >= 50 /* Greater or equal to value */
where age <= 50 /* less than or equal to a value */

/* multiple row selctions can be done aswell */
select * from people where age > 50 and height > 170
/* return all the data of people who are both aged over 50 and taller than 170 (cm) */
/* returns the all the row data, from the people table, if both the value of the
age cloumn is greater than 50 and the value of the height column is greater than 170 */

/* other logic operators can be used aswell*/
select * from people where age > 50 or height > 170
/* return all row data of people who are both aged over 50 or taller than 170 (cm) */
/* returns all the row data, from the people table, if either the value of the
age cloumn is greater than 50 or the value of the height column is greater than 170 */

/* Strings can be compared using quotes */
select * from people where first_name = "John"
/* return all row data of people who's first name is "John" */
/* returns all the row data, from the people table, if the value of the
first name cloumn is exactly "John" */

/* Strings can be compared without being exactly equal using like keyword*/
select * from people where first_name like "John"
/* return all row data of people who's first name similar to "John" */
/* returns all the row data, from the people table, if either the value of the
first name cloumn is similar to "John". In this case; "john", 'jon", "joh", "bohn"
will all also be returned as they are quite similar to john. this is keyword acts
ignores capitalisations of letters */

/* a specific substring can also be searched*/
select * from people where first_name like "%John%"
/* return all row data of people who's first name contains a similar phrase to "John" inside */
/* this will also return names like "Adam-john", "The john", "John-Carter" and "Johnny boy".
as they all contain the phrase "john" at somepoint in the string */

select * from people where full_name like "John%"
/* return all row data of people who's full name contains a similar phrase to "John"*/
/* any person with a full name that begins with a similar phrase to John will be returned
e.g. "John Smith", "John Fisher", "Jon Tomato", and "Joh the king" */

select * from people where full_name like "%Fisher"
/* return all row data of people who's full name ends with a similar phrase to "John"*/
/* any person with a full name end with a similar phrase to "Fisher" will be returned
e.g. "Adam Fisher", "Fred Fisher", "Lisa smith-Fisher", and "John Big-man kingfisher"
lisa fisher-smith would not be return in this case. */

/* percent signs can also be used in the middle of strings */
select * from people where first_name = "A%n"
/* returns all row data of people who's first name beings with 'A' and ends with "n"
adrian, Arron, adryan and alan will be return.
if you use like instead of the = sign, names like Adam, Ban, and Bam will be returned

/* underscore can be used to specific a single additonal character that may be anything */
select * from people where first_name = "A_a&"
/* names like Adam, Alan, Adaran Ataboy will be return
remeber the equal sign here means the the name must match the capitalisation of the search term.
using like will return adam, alan, adAran but also Boat, Biado, aubird as they are
simiar enough to fit the pattern */

/* can be joined with the 'and' or 'or' keywords */
select * from people where first_name = "A%" and last_name = "A&"
/* returns row data of people whos first and last name begin with A, e.g.
first_name      last_name
Adam            Adrian
Ada             Atamov      etc */
select * from people where first_name = "A%" or first_name = "B&"
/* returns row data of people whos first name begin with A or B */

/* the 'In' keyworld allows you to specifcy multiple or states e.g. */
/* CAN ONLY BE USED FOR EXACT MATCHES, % or _ CAN NOT BE USED */
select * from people where first_name in ("John", "Adam", "Bob", "Carter", "Fred", "Tim", "Mike")
/* return row data form people named john, adam, bob or carter
useful if selection criteria is long
otherwise the query would be:
select * from people where first_name = "john" or first_name = "Adam" or
    first_name = "bob" or first_name = "Carter" or  first_name = "Fred" or
    first_name = "Tim" or first_name = "Mike"

/* the 'between' operater can be used on integers */
select * from people where hieght between 150 and 170
/* returns row data from people who's hieght is > 150 and < 170
Easier and more readable than writing:
select * from people where hieght > 150 and height < 170 */

/* null keyword is used when a field in a table is empty, i.e. has no value */
select * from people where hieght is null
/* returns row data from people who DO NOT HAVE hieght data in the table */

/* not keyword can inverse a keyword or condition */
select * from people where hieght is not null
/* returns row data from people who's DO HAVE hieght data in the table */

select * from people where hieght Not between 150 and 170
/* returns row data from people who's is not hieght is > 150 and < 170
i.e returns row data from people who's is hieght is < 150 and > 170
notice how the not key word functionally invert the > and < operators */

/* you can sort using 'Order' keyword */
select * from people order by hieght
/* returns all the row data from the people table, sorted by height ascending
ascending is the default sorting order. this can be changed to descending using
the 'desc' keyword like so; */
select * from people order by hieght desc

/* getting data from multiple tables */
select
    table_1_name.name_column,
    table_2_name.job_column,
from
    table_1_name
    inner join table_2 on table_1.id_column = table_2.id_column
/* retrives data from the name column in table 1 and job column in table 2,
after joining the two tables together,
overlapping on the id column in table one with the id column in table 2.
Another example:  */

select
    people.full_name,
    companies.type_of_company,
from
    people
    inner join companies on people.current_employer = companies.company_name

/* join the people table to the companies table,
where the current empolyer and company name column overlap,
so you can extract every one's full name from the people table,
then by going through the current employer column into the companies table,
and get the associated field of business, e.g;

-----------------------------    -------------------------------
|   ::  People Table  ::    |    |  ::  companies table ::     |
----------------------------     -------------------------------
| full name   |  employer   |    | company name|type of company|
-----------------------------    -------------------------------
| John Adams  |  GlobalBank |    | GlobalBank  |  Banking      |
| lisa Fisher |  MedicalCo  |    | MedicalCo   |  Healthcare   |
-----------------------------    -------------------------------

two seperate and distinct tables that can be joined together because they share
a common column; Employer == company name. result of query would be;

+--------------------------------+
| full name   |  type of company |
+--------------------------------+
| John Adams  |  Banking         |   // John Adam works in banking
| Lisa Fisher |  Healthcare      |   // Lisa fisher works in healthcare
+--------------------------------+   // etc */

/* alias can also be used as below */
select
    p.full_name,
    c.type_of_company,
from
    people p
    inner join companies c on p.current_employer = c.company_name
/* we give people table the alias of p, and the companies table the alias of c,
making the queery much cleaner */

/* we can agregate data using additional keyword */
select
    o.outlet_name
    o.location
    sum(f.yearly_revenue) as total_revenue
from
    outlets o
    inner join financies f on o.outlet_name = f.outlet_name
    group by o.outlet_name, o.location
    order by total_revenue desc

/* retrives each outlets name, location, total employees from outlet table
+ yearly revneues form fianances table, and adds together the all the values
from the yearly revenues column

/* group by seperates the data so that not all the values are added together
only the yearly values form a single outlet, are added together, e.g.
This date below here;
|----------------------------------|     |---------------------------------|
|         financies table          |     |        outlets table            |
|----------------------------------|     |---------------------------------|
| outlet     year    yearly_revenue|     | outlet     location   employees |
|----------------------------------|     |---------------------------------|
| outlet_1   2020     10,000       |     | outlet_1   Downtown    10       |
| outlet_1   2021     11,000       |     | outlet_2   midtown     12       |
| outlet_1   2022     8,000        |     |---------------------------------|
| outlet_2   2020     12,000       |
| outlet_2   2021     13,000       |
| outlet_2   2022     9,000        |
|----------------------------------|


will be turned into this data below here
|------------------------------------|
| outlet   location   total_revenue  |
|------------------------------------|
| outlet2  midtown     34,000        |
| outlet1  downtown    29,000        |
|------------------------------------|

/* other key words to aggragate data include 'count' and 'avg' */
select
    o.outlet_name
    o.location
    sum(f.yearly_revenue) as total_revenue
    count(f.yearly_revenue) as years_operating
    avg(f.yearly_revenue) as average_revneue
from
    outlets o
    inner join financies f on o.outlet_name = f.outlet_name
    group by o.outlet_name, o.location
    order by average_revenue desc

/* 'having' lets you appply conditions to columns but only applies to numerical values */
select
    o.outlet_name
    o.location
    sum(f.yearly_revenue) as total_revenue
    count(f.yearly_revenue) as years_operating
    avg(f.yearly_revenue) as average_revenue
from
    outlets o
    inner join financies f on o.outlet_name = f.outlet_name
    group by o.outlet_name, o.location
    having avarage_revenue > 10,000
    order by average_revenue desc