-- Seed users table
INSERT INTO users (name, username, email, password, age, rights, profile_photo, banned_until, bio) VALUES
('Test User', 'testUser', 'user@example.com', '$2y$12$qmzHaCtU0KoEIt/HIQ9Blu.Lz6GOlWXXyFLLyqK.5Sv2gfeTb.DBK', 22, 'user', 'https://www.perfocal.com/blog/content/images/2021/01/Perfocal_17-11-2019_TYWFAQ_100_standard-3.jpg', NULL, ''),
('Administrator', 'admin', 'admin@example.com', '$2y$12$qmzHaCtU0KoEIt/HIQ9Blu.Lz6GOlWXXyFLLyqK.5Sv2gfeTb.DBK', 20, 'admin', 'https://images.ctfassets.net/h6goo9gw1hh6/2sNZtFAWOdP1lmQ33VwRN3/24e953b920a9cd0ff2e1d587742a2472/1-intro-photo-final.jpg', NULL, 'Bio'),
('Moderator', 'moderator', 'moderator@example.com', '$2y$12$qmzHaCtU0KoEIt/HIQ9Blu.Lz6GOlWXXyFLLyqK.5Sv2gfeTb.DBK', 21, 'moderator', 'https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png', NULL, 'Bio here');

-- Seed groups table
INSERT INTO groups (name, adminUserId) VALUES
('Test Group', (SELECT userId FROM users WHERE username = 'admin'));

-- Seed user_in_groups table
INSERT INTO userInGroup (userId, groupId, approved) VALUES
((SELECT userId FROM users WHERE username = 'testUser'), (SELECT groupId FROM groups WHERE name = 'Test Group'), 'no'),
((SELECT userId FROM users WHERE username = 'admin'), (SELECT groupId FROM groups WHERE name = 'Test Group'), 'yes');

-- Seed posts table
INSERT INTO posts (photo, description, date, postCreatorId, visibility) VALUES
('https://plus.unsplash.com/premium_photo-1664474619075-644dd191935f?fm=jpg&q=60&w=3000&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxzZWFyY2h8MXx8aW1hZ2V8ZW58MHx8MHx8fDA%3D', 'This is just test description', NOW(), (SELECT userId FROM users WHERE username = 'testUser'), 'public'),
('https://images.unsplash.com/photo-1726610930930-0e1af5f2d038?q=80&w=2071&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D', 'This is just test description 2', NOW(), (SELECT userId FROM users WHERE username = 'moderator'), 'public');

-- Seed walls table
INSERT INTO walls (userId, groupId) VALUES
((SELECT userId FROM users WHERE username = 'testUser'), NULL),
(NULL, (SELECT groupId FROM groups WHERE name = 'Test Group'));

-- Seed post_walls table
INSERT INTO postWall (wallId, postId) SELECT wallId, postId FROM walls, posts LIMIT 1;

-- Seed likes table
INSERT INTO likes (userId, postId) VALUES
((SELECT userId FROM users WHERE username = 'testUser'), (SELECT postId FROM posts WHERE postCreatorId = (SELECT userId FROM users WHERE username = 'moderator')));

-- Seed comments table
INSERT INTO comments (content, userId, postId) VALUES
('Test content', (SELECT userId FROM users WHERE username = 'admin'), (SELECT postId FROM posts WHERE postCreatorId = (SELECT userId FROM users WHERE username = 'moderator')));

-- Seed tags table
INSERT INTO tags (name) VALUES ('testTag');

-- Seed tags_in_posts table
INSERT INTO tagsInPost (tagId, postId) VALUES
((SELECT tagId FROM tags WHERE name = 'testTag'), (SELECT postId FROM posts LIMIT 1));
