create database if not exists lambda;

use lambda;

create table if not exists request (
    id integer auto_increment primary key,
    document char(11) not null,
    valid boolean not null,
    date_ datetime not null default now()
);

# TODO: There must be a way of loading a custom api
# password from an environment
create user api identified by 'lotus-waters';

grant select, insert on lambda.request to api;

flush privileges;