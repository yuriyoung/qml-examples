CREATE TABLE IF NOT EXISTS books (
    `id` INTEGER PRIMARY KEY AUTOINCREMENT,
    `title` VARCHAR(255) NOT NULL DEFAULT '',
    `isdn` VARCHAR DEFAULT '',
    `author` VARCHAR DEFAULT '',
    `publisher` VARCHAR DEFAULT '',
    `time` DATETIME NULL,
    `page` INTEGER DEFAULT 0,
    `price` INTEGER DEFAULT 0,
    `cover` VARCHAR(255) DEFAULT '',
    `description` TEXT DEFAULT '',
    `favorited` BOOLEAN DEFAULT false,
    `rating` INTEGER DEFAULT 1,
    `read` INTEGER DEFAULT 0,
    `state` INTEGER NOT NULL DEFAULT 0,
    `created_at` DATETIME NOT NULL DEFAULT (datetime('now', 'localtime')),
    `updated_at` DATETIME NOT NULL DEFAULT (datetime('now', 'localtime')),
    `deleted_at` DATETIME NULL,

    CONSTRAINT rating_1_to_5 CHECK (rating IS NULL OR (rating >= 1 and rating <= 5))
);
INSERT INTO books values(1, 'C++程序设计语言', '9787111298854', 'Bjarne Stroustrup',
'机械工业出版社', ' 2010-3-1', 905, 9900, 'https://img1.doubanio.com/view/subject/l/public/s4349507.jpg',
'本书是在C++语言和程序设计领域具有深远影响、畅销不衰的著作，由C++语言的设计者编写，对C++语言进行了最全面、最权威的论述，覆盖标准C++以及由C++所支持的关键性编程技术和设计技术。本书英文原版一经面世，即引起业内人士的高度评价和热烈欢迎，先后被翻译成德、希、匈、西、荷、法、日、俄、中、韩等近20种语言，数以百万计的程序员从中获益，是无可取代的C++经典力作。',
false, 1, 0, 0, datetime('now', 'localtime'), datetime('now', 'localtime'), NULL);
INSERT INTO books values(2, 'More Effective C++', '9787121125706', '梅耶(Scott Meyers)', '电子工业出版社', '2011-1-1',
317, 5900, 'https://img1.doubanio.com/view/subject/l/public/s28272918.jpg', '《More Effective C++:35个改善编程与设计的有效方法(中文版)》是梅耶尔大师Effective三部曲之一。继Effective C++之后，Scott Meyers于1996推出这本《More Effective C++(35个改善编程与设计的有效方法)》“续集”。',
false, 1, 0, 0, datetime('now', 'localtime'), datetime('now', 'localtime'), NULL);
