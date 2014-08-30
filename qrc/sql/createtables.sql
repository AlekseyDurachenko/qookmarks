CREATE TABLE  IF NOT EXISTS TTag(
    id integer primary key,
    parentId integer,
    tagName text,
    type integer
);
