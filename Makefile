all:
	docker compose up --build
bg:
	docker compose up --build -d 
down:
	docker compose down
fclean:
	docker compose down
	docker rmi web_server_42_img