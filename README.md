# STAGE BASE 0:

- GENERAL BASE

	Lexer/Tokenizer --> Parsing --> Abstract Syntax Tree OR Action Tree --> Compiling using LLVM or Transpiling(translating and compiling)

	-- https://www.freecodecamp.org/news/the-programming-language-pipeline-91d3f449c919/ - Steps of creating a language.


# STAGE 1 SYNTAX-PLAN:

- DECIDE SYNTAX

	This language will be a mock language for pseudocode.

	CASE SENSITIVE WITH PSEUDOCODE GRAMMER.

	https://learnlearn.uk/alevelcs/wp-content/uploads/sites/20/2020/09/9608_PSEUDOCODE_GUIDE.pdf


# STAGE 2: Writing My Own Lexer:



- LEXER STRATEGY

	Create words Letter by Letter. Tokenize words into Keywords, Operators, String, Numbers, Identifiers.

	Strings ruined things a bit since they can have spaces between them.


	-- https://archive.is/20230312193039/https://medium.com/young-coder/how-i-wrote-a-lexer-39f4f79d2980
	-- https://accu.org/journals/overload/26/145/balaam_2510/


# STAGE 3 Writing My own Parser Tree:


- PARSER TREE STRATEGY
	
	Parser Tree to what I understand is Breaking Down of expressions. Also known as Recursive Descend Parser.

	1. To create one create operant priority.
	
	2. Brackets, AND, OR, NOT, EQUAL, GREATER THAN, LESSER THAN, GREATER THAN EQUAL TO, LESSER THAN EQUAL TO, NOT EQUAL TO, Division, Multiplication, Addition, Subtraction

	3. Create A tree with 3 pointer. Data, left, right.

	