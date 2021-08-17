import "reflect-metadata";
import { createConnection, ConnectionOptions } from "typeorm";
import * as express from "express";
import { Request, Response, NextFunction } from "express";

const bodyParser = require('body-parser')
require('dotenv').config();

// backend
//
import { appRoutes } from "./routes";
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
import { Displacement } from "./entity/Displacement";
import { CurveMemberResult } from "./entity/CurveMemberResult";

// logging
import logger from "./logging/Logger"
import morganConfig from './logging/MorganConfig'

const port = Number(process.env.APP_PORT) || 3003;

const databaseOptions: ConnectionOptions = {
    type: "postgres",
    host: process.env.DATABASE_HOST!,
    port: Number(process.env.DATABASE_PORT),
    username: process.env.DATABASE_USERNAME!,
    password: process.env.DATABASE_PASSWORD!,
    database: process.env.DATABASE_NAME!,
    logging: true,
    synchronize: true,
    entities: [Project, StructuralPoint, Material, CrossSection, LoadGroup, LoadCase, PointAction, CurveMember, PointSupport, Displacement, CurveMemberResult],
};

createConnection(databaseOptions).then(async connection => {
    // create express app
    const app = express();

    app.use(bodyParser.json());
    app.use(morganConfig);

    app.use(function (req, res, next) {
        res.setHeader('Access-Control-Allow-Origin', 'http://localhost:3000');
        res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
        res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
        next();
    });

    // register all application routes
    appRoutes.forEach(route => {
        logger.info(`register endpoint: ${route.path}`);
        app[route.method](route.path, (request: Request, response: Response, next: NextFunction) => {
            route.action(request, response)
                .then(() => next)
                .catch(err => next(err));
        });
    });

    app.listen(port, () => logger.info(`sec21::backend is up and running on port: ${port}`));

}).catch(error => logger.error(`TypeORM connection error: ${error}`));
