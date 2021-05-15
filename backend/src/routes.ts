import { listProjects } from "./controller/ListProjects";
import { addProject } from "./controller/AddProject";
import { listStructuralPoints } from "./controller/ListStructuralPoints"
import { addStructuralPoint } from "./controller/AddStructuralPoint";
import { listMaterials } from "./controller/ListMaterials";
import { listCrossSections } from "./controller/ListCrossSections";
import { getProject } from "./controller/GetProject";
import { getMaterial } from "./controller/GetMaterial";
import { getCrossSection } from "./controller/GetCrossSection";
import { deleteStructuralPoint } from "./controller/DeleteStructuralPoint";

export const AppRoutes = [
    {
        path: "/projects",
        method: "get",
        action: listProjects
    },
    {
        path: "/projects",
        method: "post",
        action: addProject
    },
    {
        path: "/structuralpoints/:id",
        method: "get",
        action: listStructuralPoints
    },
    {
        path: "/materials/:id",
        method: "get",
        action: listMaterials
    },
    {
        path: "/crosssections/:id",
        method: "get",
        action: listCrossSections
    },
    {
        path: "/structuralpoints/:id",
        method: "post",
        action: addStructuralPoint
    },
    {
        path: "/structuralpoint/:id",
        method: "delete",
        action: deleteStructuralPoint
    },
    {
        path: "/project/:id",
        method: "get",
        action: getProject
    },
    {
        path: "/material/:id",
        method: "get",
        action: getMaterial
    },
    {
        path: "/crosssection/:id",
        method: "get",
        action: getCrossSection
    },
];