// todo: generate from backend ORM entities

export interface DbProject {
    id: number;
    name: string;
    title: string;
}

export interface DbStructuralPoint {
    id: number;
    name: string;
    uuid: string;
    coordinate_x: number;
    coordinate_y: number;
    coordinate_z: number;
}

export interface DbMaterial {
    id: number;
    name: string;
    uuid: string;
    e_modules: number;
}

export interface DbCrossSection {
    id: number;
    name: string;
    uuid: string;
    area: number;
}
