CREATE TABLE  IF NOT EXISTS TTag(
    id integer primary key,
    parentId integer,
    tagName text,
    type integer
);

---NEXT---

CREATE TABLE  IF NOT EXISTS TBookmark(
    id integer primary key,
    title text,
    link text
);

---NEXT---

CREATE TABLE  IF NOT EXISTS TBookmarkTag(
    id integer primary key,
    TBookmarkId integer,
    TTagId integer
);
