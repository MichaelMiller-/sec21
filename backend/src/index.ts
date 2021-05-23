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
import { PointAction } from "./entity/PointAction";
import { LoadCase } from "./entity/LoadCase";
import { LoadGroup } from "./entity/LoadGroup";
import { CurveMember } from "./entity/CurveMember";
import { PointSupport } from "./entity/PointSupport";

const PORT = Number(process.env.PORT) || 3003;
// const OPTIONS: ConnectionOptions = {
//     type: "sqlite",
//     database: process.env.DATABASE_NAME,
//     logging: true,
//     synchronize: true,
//     entities: [Project, StructuralPoint, Material, CrossSection],
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
    entities: [Project, StructuralPoint, Material, CrossSection, LoadGroup, LoadCase, PointAction, CurveMember, PointSupport],
};

createConnection(OPTIONS).then(async connection => {
    // create express app
    const app = express();
    app.use(bodyParser.json());

    app.use(function (req, res, next) {
        res.setHeader('Access-Control-Allow-Origin', 'http://localhost:3000');
        res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
        res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
        // res.setHeader('Access-Control-Allow-Credentials', true);
        next();
    });

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
