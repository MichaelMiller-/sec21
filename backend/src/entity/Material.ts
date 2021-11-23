import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, OneToMany } from "typeorm";
import { CrossSection } from "./CrossSection";
import { Project } from "./Project";
import {SurfaceMember} from "./SurfaceMember";

@Entity()
export class Material {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @Column("real")
   E_modulus: number;

   @ManyToOne(type => Project, project => project.materials)
   project: Project;

   @OneToMany(type => CrossSection, crosssection => crosssection.material)
   crossSections: CrossSection[];

   @OneToMany(type => SurfaceMember, surface => surface.material)
   surfaceMember: SurfaceMember[];
}