import {MigrationInterface, QueryRunner} from "typeorm";

export class FixRelationshipBetweenCrossSectionAndMaterial1635404854427 implements MigrationInterface {
    name = 'FixRelationshipBetweenCrossSectionAndMaterial1635404854427'

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "cross_section" DROP CONSTRAINT "FK_a443184b15fcea0e40d1148997c"`);
        await queryRunner.query(`ALTER TABLE "cross_section" DROP CONSTRAINT "REL_a443184b15fcea0e40d1148997"`);
        await queryRunner.query(`ALTER TABLE "cross_section" ADD CONSTRAINT "FK_a443184b15fcea0e40d1148997c" FOREIGN KEY ("materialId") REFERENCES "material"("id") ON DELETE NO ACTION ON UPDATE NO ACTION`);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "cross_section" DROP CONSTRAINT "FK_a443184b15fcea0e40d1148997c"`);
        await queryRunner.query(`ALTER TABLE "cross_section" ADD CONSTRAINT "REL_a443184b15fcea0e40d1148997" UNIQUE ("materialId")`);
        await queryRunner.query(`ALTER TABLE "cross_section" ADD CONSTRAINT "FK_a443184b15fcea0e40d1148997c" FOREIGN KEY ("materialId") REFERENCES "material"("id") ON DELETE NO ACTION ON UPDATE NO ACTION`);
    }

}
