import { listStructuralPoints } from "./controller/ListStructuralPoints"
import { addStructuralPoint } from "./controller/AddStructuralPoint";
import { listProjects } from "./controller/ListProject";

export const AppRoutes = [
    {
        path: "/projects",
        method: "get",
        action: listProjects
    },    
    {
        path: "/structuralpoints/:id",
        method: "get",
        action: listStructuralPoints
    },
    {
        path: "/structuralpoints/:id",
        method: "post",
        action: addStructuralPoint
    }
];