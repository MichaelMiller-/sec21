import "reflect-metadata";
import { createConnection, ConnectionOptions } from "typeorm";
import * as express from "express";
import { Request, Response, NextFunction } from "express";

const bodyParser = require('body-parser')
require('dotenv').config();

// backend
//
import { AppRoutes } from "./routes";
// entities
import { StructuralPoint } from "./entity/StructuralPoint";
import { Project } from "./entity/Project";
import { Material } from "./entity/Material";
import { CrossSection } from "./entity/CrossSection";

const PORT = Number(process.env.PORT) || 3003;
// const OPTIONS: ConnectionOptions = {
//     type: "sqlite",
//     database: process.env.DATABASE_NAME,
//     logging: true,
//     synchronize: true
//     entities: [Project, StructuralPoint],
// };

const OPTIONS: ConnectionOptions = {
    type: "postgres",
    host: process.env.DATABASE_HOST!,
    port: Number(process.env.DATABASE_PORT),
    username: process.env.DATABASE_USERNAME!,
    password: process.env.DATABASE_PASSWORD!,
    database: process.env.DATABASE_NAME!,
    logging: true,
    synchronize: true,
    entities: [Project, StructuralPoint, Material, CrossSection],
};

createConnection(OPTIONS).then(async connection => {
    // create express app
    const app = express();
    app.use(bodyParser.json());

    // register all application routes
    AppRoutes.forEach(route => {
        app[route.method](route.path, (request: Request, response: Response, next: NextFunction) => {
            route.action(request, response)
                .then(() => next)
                .catch(err => next(err));
        });
    });

    app.listen(PORT);
    console.log("sec21::backend is up and running on port:", PORT);

}).catch(error => console.log("TypeORM connection error: ", error));
