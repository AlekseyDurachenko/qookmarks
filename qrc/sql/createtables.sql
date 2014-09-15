CREATE TABLE  IF NOT EXISTS TTag(
    id integer primary key,
    parentId integer,
    name text,
    type integer,
    UNIQUE(parentId, name)
);

---NEXT---

CREATE TABLE  IF NOT EXISTS TBookmark(
    id integer primary key,
    title text,
    url text,
    UNIQUE(url)
);

---NEXT---

CREATE TABLE  IF NOT EXISTS TBookmarkTag(
    id integer primary key,
    TBookmarkId integer,
    TTagId integer,
    UNIQUE (TBookmarkId, TTagId),
    FOREIGN KEY(TBookmarkId) REFERENCES TBookmark(id),
    FOREIGN KEY(TTagId) REFERENCES TTag(id)
);
